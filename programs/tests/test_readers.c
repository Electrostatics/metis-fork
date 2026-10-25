#include "metisbin.h"

int main(int argc, char **argv)
{
  params_t params = {0};
  FILE *file;
  char filename[128];
  int graph, weighted, i;
  idx_t large = IDX_MAX/2 + 2;

  if(argc != 3)
    return 2;
  graph = strcmp(argv[1], "graph") == 0;
  weighted = strcmp(argv[2], "weighted") == 0;
  sprintf(filename, "reader-%s-%s.txt", argv[1], argv[2]);
  params.filename = filename;
  file = fopen(filename, "w");
  if(file == NULL)
    return 2;
  if(strcmp(argv[2], "overflow") == 0) {
    if(graph)
      fprintf(file, "4 1 10 %"PRIDX"\n", large);
    else
      fprintf(file, "4 %"PRIDX"\n", large);
    fputs("1 1 1 1 1 1\n1\n1\n1\n", file);
  }
  else if(strcmp(argv[2], "header-range") == 0) {
    fputs(graph ? "2 1 10 999999999999999999999999999999\n"
                : "2 999999999999999999999999999999\n", file);
  }
  else if(strcmp(argv[2], "weight-range") == 0) {
    fputs(graph ? "2 1 10 1\n999999999999999999999999999999 2\n1 1\n"
                : "2 1\n999999999999999999999999999999 1 2\n1 2 3\n", file);
  }
  else if(strcmp(argv[2], "index-limit") == 0) {
    if(graph)
      fprintf(file, "%"PRIDX" 1\n", (idx_t)IDX_MAX);
    else
      fputs("2 2\n1 2 3\n1 2 3\n", file);
  }
  else if(strcmp(argv[2], "negative") == 0) {
    fputs(graph ? "2 1 10 -1\n1 2\n1 1\n" : "2 -1\n1 2\n2 3\n", file);
  }
  else if(strcmp(argv[2], "truncated") == 0) {
    fputs(graph ? "2 1 10 2\n1\n" : "2 1\n\n1 2\n", file);
  }
  else if(strcmp(argv[2], "negative-weight") == 0) {
    fputs(graph ? "2 1 10 1\n-1 2\n1 1\n" : "2 1\n-1 1 2\n1 2 3\n", file);
  }
  else {
    fputs(graph
        ? (weighted ? "2 1 10 2\n3 4 2\n5 6 1\n" : "2 1\n2\n1\n")
        : (weighted ? "2 1\n3 1 2\n5 2 3\n" : "2\n1 2\n2 3\n"), file);
  }
  fclose(file);

  if(graph) {
    graph_t *g = ReadGraph(&params);
    if(g->nvtxs != 2 || g->nedges != 2 || g->ncon != (weighted ? 2 : 1) ||
        g->xadj[2] != 2 || g->adjncy[0] != 1 || g->adjncy[1] != 0)
      return 1;
    for(i=0; i<g->nvtxs*g->ncon; i++)
      if(g->vwgt[i] != (weighted ? i+3 : 1))
        return 1;
    FreeGraph(&g);
  }
  else {
    mesh_t *m = ReadMesh(&params);
    if(m->ne != 2 || m->nn != 3 || m->ncon != 1 || m->eptr[2] != 4 ||
        m->eind[0] != 0 || m->eind[3] != 2 ||
        m->ewgt[0] != (weighted ? 3 : 1) ||
        m->ewgt[1] != (weighted ? 5 : 1))
      return 1;
    FreeMesh(&m);
  }
  remove(filename);
  return 0;
}
