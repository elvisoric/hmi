#ifndef NRG_OBJ_LOADER_H
#define NRG_OBJ_LOADER_H
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace nrg {
struct vec3 {
  float x;
  float y;
  float z;
};

struct vec2 {
  float x;
  float y;
};

struct ObjData {
  std::vector<float> vertices;
  std::vector<float> textures;
  std::vector<float> normals;
  std::vector<unsigned int> indices;
};

std::vector<std::string> split(const std::string& s, char delimiter) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(s);
  while (std::getline(tokenStream, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}

bool startsWith(const std::string& str, const std::string& with) {
  return str.rfind(with, 0) == 0;
}
vec3 parseVec3(const std::string& data) {
  auto values = split(data, ' ');
  return {std::stof(values[0]), std::stof(values[1]), std::stof(values[2])};
}

// glm::vec3 parseNormal(const std::string& data) { return parseVertex(data); }

vec2 parseVec2(const std::string& data) {
  auto values = split(data, ' ');
  return {std::stof(values[0]), std::stof(values[1])};
}

vec3 parseFace(const std::string& data) {
  auto v = split(data, '/');
  return {std::stof(v[0]), std::stof(v[1]), std::stof(v[2])};
}

ObjData load(const std::string& filepath) {
  try {
    std::ifstream file{filepath};
    std::string line;
    std::vector<vec3> vertices;
    std::vector<vec2> textures;
    std::vector<vec3> normals;
    std::vector<unsigned int> indices;
    // std::vector<unsigned int> textureIndices;
    // std::vector<unsigned int> normalIndices;
    std::vector<float> texturesArray;
    std::vector<float> normalsArray;
    while (std::getline(file, line)) {
      if (startsWith(line, "v ")) {
        auto vertex = parseVec3(line.substr(2));
        vertices.push_back(vertex);
      } else if (startsWith(line, "vt ")) {
        auto tex = parseVec2(line.substr(3));
        textures.push_back(tex);
      } else if (startsWith(line, "vn ")) {
        auto normal = parseVec3(line.substr(3));
        normals.push_back(normal);
      } else if (startsWith(line, "f ")) {
        texturesArray.resize(vertices.size() * 2);
        normalsArray.resize(vertices.size() * 3);
        break;
      }
    }

    do {
      std::vector<std::string> vec = split(line.substr(2), ' ');
      vec3 first = parseFace(vec[0]);
      unsigned int index1 = first.x - 1;
      indices.push_back(index1);
      unsigned int texIndex1 = first.y - 1;
      const auto& tex1 = textures.at(texIndex1);
      texturesArray[index1 * 2] = tex1.x;
      texturesArray[index1 * 2 + 1] = tex1.y;

      unsigned int normIndex1 = first.z - 1;
      const auto& norm1 = normals.at(normIndex1);
      normalsArray[index1 * 3] = norm1.x;
      normalsArray[index1 * 3 + 1] = norm1.y;
      normalsArray[index1 * 3 + 2] = norm1.z;

      vec3 second = parseFace(vec[1]);
      unsigned int index2 = second.x - 1;
      indices.push_back(index2);
      unsigned int texIndex2 = second.y - 1;
      const auto& tex2 = textures.at(texIndex2);
      texturesArray[index2 * 2] = tex2.x;
      texturesArray[index2 * 2 + 1] = tex2.y;

      unsigned int normIndex2 = second.z - 1;
      const auto& norm2 = normals.at(normIndex2);
      normalsArray[index2 * 3] = norm2.x;
      normalsArray[index2 * 3 + 1] = norm2.y;
      normalsArray[index2 * 3 + 2] = norm2.z;

      vec3 third = parseFace(vec[2]);
      unsigned int index3 = third.x - 1;
      indices.push_back(index3);
      unsigned int texIndex3 = third.y - 1;
      const auto& tex3 = textures.at(texIndex3);
      texturesArray[index3 * 2] = tex3.x;
      texturesArray[index3 * 2 + 1] = tex3.y;

      unsigned int normIndex3 = third.z - 1;
      const auto& norm3 = normals.at(normIndex3);
      normalsArray[index3 * 3] = norm3.x;
      normalsArray[index3 * 3 + 1] = norm3.y;
      normalsArray[index3 * 3 + 2] = norm3.z;
    } while (getline(file, line));

    std::vector<float> vertexData;
    for (const auto& v : vertices) {
      vertexData.push_back(v.x);
      vertexData.push_back(v.y);
      vertexData.push_back(v.z);
    }
    ObjData result;
    result.vertices = vertexData;
    result.indices = indices;
    result.textures = texturesArray;
    result.normals = normalsArray;
    return result;
  } catch (std::exception& ex) {
    std::cout << "exception: " << ex.what() << std::endl;
    throw;
  }
}

}  // namespace nrg
#endif  // NRG_OBJ_LOADER_H