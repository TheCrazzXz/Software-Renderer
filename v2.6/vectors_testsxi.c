#include "memory/vector.h"
#include "maths/vec.h"
#include <stdio.h>

struct TestStruct
{
  Vector3 vec3;
  Vector4 vec4;
  Vector2 vec2;
  float n;
};

int main(void) {
  Vector vec = Vector_create(sizeof(struct TestStruct));
  printVector(&vec);

  for (int i = 1; i < 50; i++) 
  {

    struct TestStruct testStructElement = 
    { 
      {i, 2 * i, atan(i)},
      {i, (float)i / 2, sqrt(i), 5},
      {10 * i, 1.0 / (float)i},
      20 * i
     
    };

    Vector_Push_back(&vec, &testStructElement);
  }
  for (ulong i = 0; i < vec.length; i++) {
    struct TestStruct* element = (struct TestStruct*)Vector_element(&vec, i);
    printf("[%lu] : \n", i);
    printf("vec3 : ");
    printVector3(&element->vec3);
    printf("vec4 : ");
    printVector4(&element->vec4);
    printf("vec2 : ");
    printVector2(&element->vec2);
    printf("n : %f\n", element->n);
  }

  Vector_Delete(&vec);
}