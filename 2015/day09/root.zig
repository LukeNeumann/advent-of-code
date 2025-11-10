const std = @import("std");
const input = @embedFile("input.txt");

const EdgeKey = struct {
    from: usize,
    to: usize,
};

pub fn AddNode(nodes: *std.StringHashMap(usize), node_id: []const u8) !usize {
    const next_value = nodes.count();
    const node = try nodes.getOrPut(node_id);
    if (!node.found_existing) {
        node.value_ptr.* = next_value;
    }
    return node.value_ptr.*;
}

pub fn AddEdge(edges: *std.AutoHashMap(EdgeKey, u32), from: usize, to: usize, distance: u32) !void {
    const e1 = try edges.getOrPut(.{ .from = from, .to = to });
    e1.value_ptr.* = distance;
    const e2 = try edges.getOrPut(.{ .from = to, .to = from });
    e2.value_ptr.* = distance;
}

pub fn PrintArrayList(T: type, in: std.ArrayList(T), name: []const u8) void {
    std.debug.print("{s} [", .{name});
    for (in.items, 0..) |n, i| {
        if (i > 0) std.debug.print(" ", .{});
        std.debug.print("{d}", .{n});
    }
    std.debug.print("]\n", .{});
}

pub fn solve() !void {
    var gpa: std.heap.GeneralPurposeAllocator(.{}) = .init;
    const alloc = gpa.allocator();
    defer _ = gpa.deinit();

    var edges = std.AutoHashMap(EdgeKey, u32).init(alloc);
    defer edges.deinit();
    var nodes = std.StringHashMap(usize).init(alloc);
    defer nodes.deinit();

    var it = std.mem.tokenizeScalar(u8, input, '\n');
    while (it.next()) |line| {
        var line_it = std.mem.tokenizeScalar(u8, line, ' ');
        const from_id = line_it.next().?;
        _ = line_it.next().?; // "to"
        const to_id = line_it.next().?;
        _ = line_it.next().?; // "="
        const distance = try std.fmt.parseInt(u32, line_it.next().?, 10);

        const from = try AddNode(&nodes, from_id);
        const to = try AddNode(&nodes, to_id);

        try AddEdge(&edges, from, to, distance);
    }
    // var node_it = nodes.valueIterator();
    // while (node_it.next()) |node| {
    // std.debug.print("Node {d}\n", .{node.*});
    // }
    // var edge_it = edges.iterator();
    // while (edge_it.next()) |entry| {
    //     std.debug.print("Edge[{d},{d}] = {d}\n", .{ entry.key_ptr.from, entry.key_ptr.to, entry.value_ptr.* });
    // }

    var min_distance: ?u32 = null;
    var max_distance: ?u32 = null;
    var route = try std.ArrayList(usize).initCapacity(alloc, nodes.count());
    defer route.deinit(alloc);
    var check = try std.ArrayList(bool).initCapacity(alloc, nodes.count());
    defer check.deinit(alloc);
    var node_it = nodes.valueIterator();
    while (node_it.next()) |_| {
        try route.append(alloc, 0);
        try check.append(alloc, false);
    }

    while (true) {
        const done = next_route(route.items, check.items);
        if (done) break;
        //PrintArrayList(usize, route, "route");

        var curr_distance: u32 = 0;
        var route_it = std.mem.window(usize, route.items, 2, 1);
        while (route_it.next()) |window| {
            curr_distance += edges.get(.{ .from = window[0], .to = window[1] }).?;
        }
        if (min_distance == null or curr_distance < min_distance.?) min_distance = curr_distance;
        if (max_distance == null or curr_distance > max_distance.?) max_distance = curr_distance;
    }

    std.debug.print("{d}\n{d}\n", .{ min_distance.?, max_distance.? });
}

pub fn next_route(route: []usize, check: []bool) bool {
    const count = route.len;
    var valid = false;

    while (!valid) {
        for (route, 0..) |*node, i| {
            node.* += 1;
            if (node.* == count) {
                node.* = 0;
            } else {
                break;
            }
            if (i == count - 1) return true; // done
        }
        valid = route_valid(route, check);
    }
    return false;
}

pub fn route_valid(route: []usize, check: []bool) bool {
    for (check) |*n| n.* = false;
    for (route) |n| {
        if (check[n]) {
            return false;
        }
        check[n] = true;
    }
    return true;
}
