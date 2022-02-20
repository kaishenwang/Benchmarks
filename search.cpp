#include <iostream>
#include <random>
#include <algorithm>
#include <iterator>
#include <vector>
#include <set>
#include <unordered_set>
#include <tuple>

using namespace std;

template <typename C, typename F>
uint32_t search(C container, const vector<typename C::value_type>& sources, F search_f) {
    auto begin = chrono::steady_clock::now();
    for (const auto& source : sources) {
        search_f(source, container);
    }
    auto end = chrono::steady_clock::now();
    auto elpased = chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count();
    return elpased;
}

auto run(uint32_t n) {
    // First create an instance of an engine.
    random_device rnd_device;
    // Specify the engine and distribution.
    mt19937 mersenne_engine {rnd_device()};  // Generates random integers
    uniform_int_distribution<int> dist {1, 1000000};
    
    auto gen = [&dist, &mersenne_engine](){
                   return dist(mersenne_engine);
               };

    vector<int> sources(n);
    generate(begin(sources), end(sources), gen);

    vector<int> container_v(n);
    generate(begin(container_v), end(container_v), gen);

    // Vector
    auto vector_search_f = [](auto& x, const auto& container) {
        for (const auto& v : container) {
            if (v == x) {
                return;
            }
        }
    };

    auto v_res = search(container_v, sources, vector_search_f);

    // set
    set<int> container_set (container_v.begin(), container_v.end());
    auto set_search_f = [](auto& x, const auto& container) {
        return container.find(x) != end(container);
    };
    auto s_res = search(container_set, sources, set_search_f);


    // unordered set
    unordered_set<int> container_unordered_set (container_v.begin(), container_v.end());
    auto us_res = search(container_unordered_set, sources, set_search_f);

    return make_tuple(v_res, s_res, us_res);
}

int main() {
    cout << "N,vector,set,unordered_set" << endl; 
    for (uint32_t n = 1; n < 100; ++n) {
        int v, s, us;
        tie(v, s, us)  = run(n);
        cout << n << "," << v << "," << s << "," << us << endl;
    }
    return 0;
}