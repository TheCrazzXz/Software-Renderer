#include "polygon_clipping.h"

int main(void)
{
  Vector2 subject_polygon[] = {
    {0,3},{0.5,0.5},{3,0},{0.5,-0.5},{0,-3},{-0.5,-0.5},{-3,0},{-0.5,0.5}
  };

  Vector2 clipping_polygon[] = {
    {-2, -2}, 
    {-2, 2}, 
    {2, 2},
    {2, -2}
  };
  printf("length : %lu\n", sizeof(clipping_polygon) / sizeof(Vector2));
  Vector clipped_polygon = clip(subject_polygon, sizeof(subject_polygon) / sizeof(Vector2), clipping_polygon, sizeof(clipping_polygon) / sizeof(Vector2));

  printf("Length : %lu\n", clipped_polygon.length);
  for(ulong i = 0 ; i < clipped_polygon.length ; i++)
  {
    printf("[%lu] : \n", i);
    printVector2((Vector2*)Vector_element(&clipped_polygon, i));
  }
}