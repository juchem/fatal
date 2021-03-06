#include <fatal/test/words.h>
#include <fatal/type/trie.h>

#include <iostream>
#include <string>

int main() {
  using namespace ftl;

  for (std::string needle; std::cin >> needle; ) {
    std::cout << needle << ": " << std::boolalpha
      << trie_exact_match<random_250_words<list, sequence>>(
        needle.begin(),
        needle.end()
      )
      << std::endl;
  }

  return 0;
}
