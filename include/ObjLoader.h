#ifndef NRG_OBJ_LOADER_H
#define NRG_OBJ_LOADER_H
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
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
  friend std::ostream& operator<<(std::ostream& out, const ObjData& obj) {
    out << "Vertices: " << std::endl;
    for (const auto& v : obj.vertices) {
      out << v << " ";
    }
    out << std::endl;
    out << "Normals: " << std::endl;
    for (const auto& n : obj.normals) {
      out << n << " ";
    }
    out << std::endl;
    out << "Textures: " << std::endl;
    for (const auto& t : obj.textures) {
      out << t << " ";
    }
    out << std::endl;
    out << "Indices: " << std::endl;
    for (const auto& i : obj.indices) {
      out << i << " ";
    }
    out << std::endl;
    return out;
  }
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
    std::vector<float> verticesArray;
    std::vector<float> texturesArray;
    std::vector<float> normalsArray;
    std::unordered_map<std::string, unsigned int> combinedIndices;
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
        break;
      }
    }
    unsigned int indexCount = 0;
    do {
      std::vector<std::string> vec = split(line.substr(2), ' ');
      auto iter1 = combinedIndices.find(vec[0]);
      if (iter1 != std::end(combinedIndices)) {
        indices.push_back(iter1->second);
      } else {
        vec3 first = parseFace(vec[0]);
        unsigned int index1 = first.x - 1;
        const auto& vertex = vertices[index1];
        verticesArray.push_back(vertex.x);
        verticesArray.push_back(vertex.y);
        verticesArray.push_back(vertex.z);

        unsigned int texIndex1 = first.y - 1;
        const auto& tex = textures.at(texIndex1);
        texturesArray.push_back(tex.x);
        texturesArray.push_back(1 - tex.y);

        unsigned int normIndex1 = first.z - 1;
        const auto& norm = normals.at(normIndex1);
        normalsArray.push_back(norm.x);
        normalsArray.push_back(norm.y);
        normalsArray.push_back(norm.z);

        indices.push_back(indexCount);
        combinedIndices[vec[0]] = indexCount;
        ++indexCount;
      }

      auto iter2 = combinedIndices.find(vec[1]);
      if (iter2 != std::end(combinedIndices)) {
        indices.push_back(iter2->second);
      } else {
        vec3 second = parseFace(vec[1]);
        unsigned int index2 = second.x - 1;
        const auto& vertex = vertices[index2];
        verticesArray.push_back(vertex.x);
        verticesArray.push_back(vertex.y);
        verticesArray.push_back(vertex.z);

        unsigned int texIndex2 = second.y - 1;
        const auto& tex = textures.at(texIndex2);
        texturesArray.push_back(tex.x);
        texturesArray.push_back(1 - tex.y);

        unsigned int normIndex2 = second.z - 1;
        const auto& norm = normals.at(normIndex2);
        normalsArray.push_back(norm.x);
        normalsArray.push_back(norm.y);
        normalsArray.push_back(norm.z);

        indices.push_back(indexCount);
        combinedIndices[vec[1]] = indexCount;
        ++indexCount;
      }

      auto iter3 = combinedIndices.find(vec[2]);
      if (iter3 != std::end(combinedIndices)) {
        indices.push_back(iter3->second);
      } else {
        vec3 third = parseFace(vec[2]);
        unsigned int index3 = third.x - 1;
        const auto& vertex = vertices[index3];
        verticesArray.push_back(vertex.x);
        verticesArray.push_back(vertex.y);
        verticesArray.push_back(vertex.z);

        unsigned int texIndex3 = third.y - 1;
        const auto& tex = textures.at(texIndex3);
        texturesArray.push_back(tex.x);
        texturesArray.push_back(1 - tex.y);

        unsigned int normIndex3 = third.z - 1;
        const auto& norm = normals.at(normIndex3);
        normalsArray.push_back(norm.x);
        normalsArray.push_back(norm.y);
        normalsArray.push_back(norm.z);

        indices.push_back(indexCount);
        combinedIndices[vec[2]] = indexCount;
        ++indexCount;
      }
    } while (getline(file, line));

    ObjData result;
    result.vertices = verticesArray;
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