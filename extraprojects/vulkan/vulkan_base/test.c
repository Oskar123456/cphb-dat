#include "external/cglm/include/cglm/io.h"
#include "external/cglm/include/cglm/mat4.h"
#include "external/cglm/include/cglm/simd/sse2/mat4.h"
#include "external/cglm/include/cglm/types.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

#define GLFW_INCLUDE_VULKAN
#include "external/glfw/include/GLFW/glfw3.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
/* #include <glm/vec4.hpp> */
/* #include <glm/mat4x4.hpp> */
#include "external/cglm/include/cglm/cglm.h"

int main() {
  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  GLFWwindow *window = glfwCreateWindow(800, 600, "Vulkan window", NULL, NULL);

  uint32_t extensionCount = 0;
  vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, NULL);

  printf("%d : extensions supported", extensionCount);

  mat4 matrix = {
  1,0,0,0,
  0,1,0,0,
  0,0,1,0,
  0,0,0,1
};

  mat4 copyofmatrix;
  glm_mat4_inv_precise_sse2(matrix, copyofmatrix);

  glm_mat4_print(matrix, stdout);
  glm_mat4_print(copyofmatrix, stdout);

  mat4 *dynMat = (mat4*) malloc(sizeof(float) * 16);

  glm_mat4_print(*dynMat, stdout);
  glm_mat4_identity(*dynMat);
  glm_mat4_print(*dynMat, stdout);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
  }

  glfwDestroyWindow(window);

  glfwTerminate();

  return 0;
}
