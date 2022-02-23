//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/loader/mesh_loader.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/modules/assimp/assimp_mesh_loader.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  void MeshLoader::Initialize() {
    s_mesh_loader = new AssimpMeshLoader();
  }

  //--------------------------------------------------------------
  void MeshLoader::Shutdown() {
    delete s_mesh_loader;
  }

}