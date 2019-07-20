/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#include <fatal/string/string_view.h>
#include <fatal/type/array.h>
#include <fatal/type/list.h>
#include <fatal/type/sequence.h>
#include <fatal/type/trie.h>

#include <fatal/portability.h>

#include <fatal/test/driver.h>

#include <type_traits>

namespace fatal {

FATAL_S(g, "g");
FATAL_S(go, "go");
FATAL_S(goo, "goo");
FATAL_S(h, "h");
FATAL_S(ha, "ha");
FATAL_S(hat, "hat");
FATAL_S(hi, "hi");
FATAL_S(hin, "hin");
FATAL_S(hint, "hint");
FATAL_S(hit, "hit");
FATAL_S(ho, "ho");
FATAL_S(hoT, "hoT");
FATAL_S(hoTtoo, "hoTtoo");
FATAL_S(hot, "hot");
FATAL_S(hottoo, "hottoo");

using hs_tree = list<ha, hat, hi, hint, hit, ho, hot>;

#define TEST_TRIE_FIND(Expected, Haystack, Needle) \
  do { \
    auto const needle = z_view<Needle>(); \
    bool const actual = trie_longest_match<Haystack>(needle.begin(), needle.end()); \
    FATAL_EXPECT_EQ(Expected, actual); \
  } while (false)

FATAL_TEST(trie_longest_match, longest) {
  TEST_TRIE_FIND(false, hs_tree, h);
  TEST_TRIE_FIND(true, hs_tree, ha);
  TEST_TRIE_FIND(true, hs_tree, hat);
  TEST_TRIE_FIND(true, hs_tree, hi);
  TEST_TRIE_FIND(true, hs_tree, hit);
  TEST_TRIE_FIND(true, hs_tree, hin);
  TEST_TRIE_FIND(true, hs_tree, hint);
  TEST_TRIE_FIND(true, hs_tree, ho);
  TEST_TRIE_FIND(true, hs_tree, hottoo);
  TEST_TRIE_FIND(true, hs_tree, hoT);
  TEST_TRIE_FIND(true, hs_tree, hoTtoo);
  TEST_TRIE_FIND(false, hs_tree, g);
  TEST_TRIE_FIND(false, hs_tree, go);
  TEST_TRIE_FIND(false, hs_tree, goo);
}

#undef TEST_TRIE_FIND

} // namespace fatal {
