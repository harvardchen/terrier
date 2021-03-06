#pragma once
#include <random>
#include <vector>
#include "catalog/schema.h"
#include "common/strong_typedef.h"
#include "type/type_id.h"
#include "util/random_test_util.h"
#include "util/storage_test_util.h"
#include "util/test_thread_pool.h"

namespace terrier {
struct CatalogTestUtil {
  // Returns a random Schema that is guaranteed to be valid for a single test instance. Multiple Schemas instantiated
  // with this function may have non-unique oids, but within a Schema, Column oids are unique.
  template <typename Random>
  static catalog::Schema RandomSchema(const uint16_t max_cols, Random *const generator) {
    TERRIER_ASSERT(max_cols > 0, "There should be at least 1 columm.");
    col_oid_t col_oid(0);
    const uint16_t num_attrs = std::uniform_int_distribution<uint16_t>(1, max_cols)(*generator);
    std::vector<type::TypeId> possible_attr_types{
        type::TypeId::BOOLEAN, type::TypeId::TINYINT,   type::TypeId::SMALLINT,  type::TypeId::INTEGER,
        type::TypeId::BIGINT,  type::TypeId::DECIMAL,   type::TypeId::TIMESTAMP, type::TypeId::DATE,
        type::TypeId::VARCHAR, type::TypeId::VARBINARY, type::TypeId::ARRAY};
    std::vector<bool> possible_attr_nullable{true, false};
    std::vector<catalog::Schema::Column> columns;
    for (uint16_t i = 0; i < num_attrs; i++) {
      type::TypeId attr_type = *RandomTestUtil::UniformRandomElement(&possible_attr_types, generator);
      bool attr_nullable = *RandomTestUtil::UniformRandomElement(&possible_attr_nullable, generator);
      columns.emplace_back("col_name", attr_type, attr_nullable, col_oid++);
    }
    return catalog::Schema(columns);
  }
};
}  // namespace terrier
