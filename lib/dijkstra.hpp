#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

#include "quaternion3d.hpp"
#include "vector3d.hpp"
#include "vector6d.hpp"
#include <algorithm>
#include <concepts>
#include <queue>
#include <ranges>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace Linalg {

    template <typename T>
    [[nodiscard]] auto
    dijkstra(std::unordered_map<T, std::unordered_map<T, T>> const& graph, T const& start, T const& goal)
    {
        auto locations_to_visit{make_empty_locations_to_visit(start)};
        auto locations_visited{make_empty_locations_visited(graph)};
        auto location_to_parent{make_location_to_empty_parent(graph)};
        auto location_to_cost{make_location_to_infinite_cost(graph)};

        while (!locations_to_visit.empty()) {
            auto const current_location{locations_to_visit.top()};
            locations_to_visit.pop();

            if (locations_visited.find(current_location) == locations_visited.cend()) {
                if (current_location == goal) {
                    break;
                }
                auto const& neighbor_location_to_distance{graph.at(current_location)};
                for (auto const& [neighbor_location, distance] : neighbor_location_to_distance) {
                    if (locations_visited.find(neighbor_location) == locations_visited.cend()) {
                        auto const& old_cost{location_to_cost.at(neighbor_location)};
                        auto const& distance{neighbor_location_to_distance.at(neighbor_location)};
                        auto const& new_cost{location_to_cost.at(current_location) + distance};

                        if (new_cost < old_cost) {
                            location_to_cost.at(neighbor_location) = new_cost;
                            location_to_parent.at(neighbor_location) = current_location;
                            locations_to_visit.push(neighbor_location);
                            locations_visited.insert(current_location);
                        }
                    }
                }
            }
        }

        return make_path(location_to_parent, start, goal);
    }

    template <typename T>
    static auto make_path(std::unordered_map<T, T> const& location_to_parent, T const& start, T const& goal)
    {
        std::vector<T> path{};
        path.reserve(location_to_parent.size());
        auto current_location{goal};
        while (current_location != start) {
            path.push_back(current_location);
            current_location = location_to_parent.at(current_location);
        }
        std::ranges::reverse(path);
        return path;
    }

    template <typename T>
    static auto make_location_to_empty_parent(std::unordered_map<T, std::unordered_map<T, T>> const& graph)
    {
        std::unordered_map<T, T> location_to_parent{};
        location_to_parent.reserve(graph.size());
        for (auto const& [location, neighbors] : graph) {
            // start with default initialized parent location
            location_to_parent.emplace_hint(location_to_parent.cend(), location, Location{});
        }
        return location_to_parent;
    }

    template <typename T>
    static auto make_location_to_infinite_cost(std::unordered_map<T, std::unordered_map<T, T>> const& graph)
    {
        std::unordered_map<T, T> location_to_cost{};
        location_to_cost.reserve(graph.size());
        for (auto const& [location, neighbors] : graph) {
            // start with infinity initialized cost
            location_to_cost.emplace_hint(location_to_cost.cend(), location, std::numeric_limits<Location>::max());
        }
        return location_to_cost;
    }

    template <typename T>
    static auto make_empty_locations_to_visit(T const& start)
    {
        std::priority_queue<T> locations_to_visit{};
        locations_to_visit.push(start);
        return locations_to_visit;
    }

    template <typename T>
    static auto make_empty_locations_visited(std::unordered_map<T, std::unordered_map<T, T>> const& graph)
    {
        std::unordered_set<T> locations_visited{};
        locations_visited.reserve(graph.size());
        return locations_visited;
    }
};
}
; // namespace Linalg

#endif // DIJKSTRA_HPP