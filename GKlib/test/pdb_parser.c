#include <GKlib.h>

static void write_atom(FILE *file, const char *name, int serial)
{
  fprintf(file,
      "ATOM  %5d %-4s ALA A   1    %8.3f%8.3f%8.3f%6.2f%6.2f           C\n",
      serial, name, 1.0, 2.0, 3.0, 1.0, 10.0);
}

int main(int argc, char **argv)
{
  FILE *file;
  pdbf *pdb;
  char filename[128];
  int valid, i;

  if (argc != 2)
    return 2;
  snprintf(filename, sizeof(filename), "pdb-parser-%s.pdb", argv[1]);
  file = fopen(filename, "w");
  if (file == NULL)
    return 2;
  valid = strcmp(argv[1], "valid") == 0;
  if (valid) {
    fputs("HEADER    PARSER REGRESSION\n\n", file);
    write_atom(file, "N", 1);
    write_atom(file, "CA", 2);
    write_atom(file, "C", 3);
    write_atom(file, "O", 4);
    fputs("END\n", file);
  }
  else if (strcmp(argv[1], "six-character") == 0) {
    fputs("ATOMAA    1  CA  ALA A   1       1.000   2.000   3.000  1.00 10.00\n", file);
  }
  else if (strcmp(argv[1], "long-token") == 0) {
    fputs("ATOM", file);
    for (i=0; i<200; i++)
      fputc('A', file);
    fputc('\n', file);
  }
  else if (strcmp(argv[1], "truncated") == 0) {
    fputs("ATOM      1  CA  ALA A   1\n", file);
  }
  else if (strcmp(argv[1], "empty") != 0) {
    fclose(file);
    remove(filename);
    return 2;
  }
  fclose(file);

  gk_set_exit_on_error(0);
  pdb = gk_readpdbfile(filename);
  remove(filename);
  if (valid) {
    if (pdb == NULL || pdb->natoms != 4 || pdb->nresidues != 1 ||
        pdb->ncas != 1 || pdb->nbbs != 4 || pdb->corruption != 0 ||
        strcmp(pdb->atoms[1].name, "CA") != 0 ||
        strcmp(pdb->atoms[1].resname, "ALA") != 0 ||
        pdb->atoms[1].serial != 2 || pdb->atoms[1].x != 1.0 ||
        pdb->atoms[1].y != 2.0 || pdb->atoms[1].z != 3.0 ||
        pdb->cm[0].x != 1.0 || pdb->resSeq[0] != 'A')
      return 1;
    gk_freepdbf(pdb);
  }
  else if (strcmp(argv[1], "empty") == 0) {
    if (pdb == NULL || pdb->natoms != 0 || pdb->nresidues != 0)
      return 1;
    gk_freepdbf(pdb);
  }
  else if (pdb != NULL) {
    gk_freepdbf(pdb);
    return 1;
  }
  return 0;
}
