/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_test_driver_h
#define FATAL_INCLUDE_fatal_test_driver_h

#include <fatal/log/log.h>
#include <fatal/test/args.h>
#include <fatal/test/random_data.h>
#include <fatal/test/string.h>
#include <fatal/test/test.h>
#include <fatal/test/type.h>

#include <iostream>
#include <map>
#include <string>

////////////
// DRIVER //
////////////

int main(int const argc, char const *const *const argv) {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  auto const arg_list = std::string("--list");
  auto const arg_filter = std::string("--filter");
  auto const arg_gtest = std::string("--gtest");
  auto const arg_gtest_list = std::string("--gtest_list_tests");
  auto const arg_gtest_filter = std::string("--gtest_filter");

  if (argc == 0) {
    return EXIT_FAILURE; // protect parse_args below
  }

  using Opts = std::map<std::string, std::string>;
  auto const opts = ftl::test_impl::args::parse_args<Opts>(argc, argv);

  if (opts.empty()) {
    return ftl::test::run_all<ftl::test::default_printer>(std::cerr);
  }

  if (opts.find(arg_gtest) != opts.end()) {
    return ftl::test::run_all<ftl::test::gtest_printer>(std::cout);
  }

  if (opts.find(arg_list) != opts.end()) {
    return ftl::test::list<ftl::test::default_printer>(std::cout);
  }

  if (opts.find(arg_gtest_list) != opts.end()) {
    return ftl::test::list<ftl::test::gtest_printer>(std::cout);
  }

  if (auto const iter_filter = opts.find(arg_filter); iter_filter != opts.end()) {
    return ftl::test::run_one<ftl::test::default_printer>(
      std::cerr, iter_filter->second
    );
  }

  if (auto const iter_gtest_filter = opts.find(arg_gtest_filter); iter_gtest_filter != opts.end()) {
    return ftl::test::run_one<ftl::test::gtest_printer>(
      std::cout, iter_gtest_filter->second
    );
  }

  return EXIT_FAILURE; // unrecognized input
}

#endif // FATAL_INCLUDE_fatal_test_driver_h
