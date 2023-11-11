#include "mcc/test_material.h"

namespace mcc {
  TEST_F(MaterialTest, Test_Serialization) {    
    MaterialList all_materials;
    GenerateMaterials(all_materials);
    auto buffer = ToBuffer(all_materials);

    MaterialList materials;
    FromBuffer(buffer, materials);

    ASSERT_EQ(materials.size(), all_materials.size());
    for(auto idx = 0; idx < all_materials.size(); idx++) {
      ASSERT_EQ(all_materials[idx], materials[idx]);
    }
  }
}