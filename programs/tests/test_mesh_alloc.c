#include "metislib.h"

/* Fail selected result-array allocations in mesh.c without affecting GKlib. */
static int malloc_calls = 0;
static int fail_call = 0;

static void *test_malloc(size_t size)
{
  if (++malloc_calls == fail_call)
    return NULL;
  return malloc(size);
}

#define malloc test_malloc
#include "../../libmetis/mesh.c"
#undef malloc

static int check(int nodal, int fail, int expected)
{
  /* Two quadrilaterals sharing an edge, using Fortran numbering. */
  idx_t ne = 2, nn = 6, ncommon = 2, numflag = 1;
  idx_t eptr[3] = {1, 5, 9};
  idx_t eind[8] = {1, 2, 5, 4, 2, 3, 6, 5};
  idx_t eptr0[3], eind0[8];
  idx_t *xadj = NULL, *adjncy = NULL;
  int i, rcode;

  memcpy(eptr0, eptr, sizeof(eptr));
  memcpy(eind0, eind, sizeof(eind));
  malloc_calls = 0;
  fail_call = fail;

  if (nodal)
    rcode = METIS_MeshToNodal(&ne, &nn, eptr, eind, &numflag, &xadj, &adjncy);
  else
    rcode = METIS_MeshToDual(&ne, &nn, eptr, eind, &ncommon, &numflag,
                &xadj, &adjncy);

  if (rcode != expected) {
    fprintf(stderr, "nodal=%d fail=%d: rcode %d, expected %d\n", nodal, fail,
        rcode, expected);
    return 1;
  }
  for (i=0; i<3; i++)
    if (eptr[i] != eptr0[i])
      return fprintf(stderr, "nodal=%d fail=%d: eptr changed\n", nodal, fail), 1;
  for (i=0; i<8; i++)
    if (eind[i] != eind0[i])
      return fprintf(stderr, "nodal=%d fail=%d: eind changed\n", nodal, fail), 1;

  if (expected == METIS_OK) {
    if (xadj == NULL || adjncy == NULL || xadj[0] != 1)
      return fprintf(stderr, "nodal=%d: bad graph\n", nodal), 1;
    METIS_Free(xadj);
    METIS_Free(adjncy);
  }
  else if (xadj != NULL || adjncy != NULL) {
    return fprintf(stderr, "nodal=%d fail=%d: outputs not cleared\n", nodal,
        fail), 1;
  }
  return 0;
}

int main(void)
{
  int nodal, fail, errors = 0;

  gk_set_exit_on_error(0);
  for (nodal=0; nodal<2; nodal++) {
    errors += check(nodal, 0, METIS_OK);
    for (fail=1; fail<=2; fail++)
      errors += check(nodal, fail, METIS_ERROR_MEMORY);
  }
  return errors != 0;
}
