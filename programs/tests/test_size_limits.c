#include "metislib.h"

/* An 8-vertex cycle partitioned with two balancing constraints. */
static idx_t xadj[9] = {0, 2, 4, 6, 8, 10, 12, 14, 16};
static idx_t adjncy[16] = {1, 7, 0, 2, 1, 3, 2, 4, 3, 5, 4, 6, 5, 7, 6, 0};
static idx_t vwgt[16] = {1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2};

static int partition(int recursive, idx_t nvtxs, idx_t ncon, idx_t nparts,
    idx_t *part)
{
  idx_t objval;

  if (recursive)
    return METIS_PartGraphRecursive(&nvtxs, &ncon, xadj, adjncy, vwgt, NULL,
               NULL, &nparts, NULL, NULL, NULL, &objval, part);
  return METIS_PartGraphKway(&nvtxs, &ncon, xadj, adjncy, vwgt, NULL, NULL,
             &nparts, NULL, NULL, NULL, &objval, part);
}

int main(void)
{
  idx_t part[8], i;
  int recursive, errors = 0;

  for (recursive=0; recursive<2; recursive++) {
    if (partition(recursive, 8, 2, 2, part) != METIS_OK) {
      fprintf(stderr, "recursive=%d: valid partition failed\n", recursive);
      errors++;
    }
    for (i=0; i<8; i++)
      if (part[i] < 0 || part[i] >= 2)
        errors++;

    /* These products exceed IDX_MAX and must be rejected before use. */
    if (partition(recursive, IDX_MAX/2 + 1, 2, 2, part) != METIS_ERROR_INPUT) {
      fprintf(stderr, "recursive=%d: nvtxs*ncon overflow accepted\n", recursive);
      errors++;
    }
    if (partition(recursive, 8, 2, IDX_MAX/2 + 1, part) != METIS_ERROR_INPUT) {
      fprintf(stderr, "recursive=%d: nparts*ncon overflow accepted\n", recursive);
      errors++;
    }
  }
  return errors != 0;
}
