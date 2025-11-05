const std = @import("std");
const input = @embedFile("input.txt");

const Gate = struct {
    const Self = @This();

    const OpType: type = enum {
        NOT,
        OR,
        AND,
        LSHIFT,
        RSHIFT,
        NOOP,
        CONST,
    };

    op: OpType = .NOOP,
    in1: ?usize = null,
    in2: ?usize = null,

    value: u16 = 0,
    needs_evaluation: bool = true,
    id: []const u8 = undefined,

    pub fn get_or_make_gate(alloc: std.mem.Allocator, gates: *std.ArrayList(Gate), gates_hash: *std.StringHashMap(usize), ids: *std.ArrayList([]const u8), id: []const u8) !usize {
        const get = gates_hash.get(id);
        if (get == null) {
            try ids.append(alloc, id);

            var is_const = true;
            const value = std.fmt.parseInt(u16, id, 10) catch err: {
                is_const = false;
                break :err 0;
            };
            try gates.append(alloc, .{
                .op = if (is_const) .CONST else .NOOP,
                .value = value,
                .id = ids.getLast(),
            });

            try gates_hash.put(ids.getLast(), gates.items.len - 1);
            return gates.items.len - 1;
        } else {
            return get.?;
        }
    }

    pub fn evaluate(self: *Self, gates: std.ArrayList(Gate)) u16 {
        if (!self.needs_evaluation) return self.value;

        var left_id: []const u8 = "";
        var right_id: []const u8 = "";

        var left_operand: u16 = 0;
        var right_operand: u16 = 0;
        var shift_operand: u4 = 0;

        if (self.in1) |index| {
            left_operand = gates.items[index].evaluate(gates);
            left_id = gates.items[index].id;
        }
        if (self.in2) |index| {
            right_operand = gates.items[index].evaluate(gates);
            shift_operand = @truncate(right_operand);
            right_id = gates.items[index].id;
        }

        var result: u16 = 0;
        switch (self.op) {
            .CONST => result = self.value,
            .NOOP => result = left_operand,
            .NOT => result = ~left_operand,
            .OR => result = left_operand | right_operand,
            .AND => result = left_operand & right_operand,
            .LSHIFT => result = left_operand << shift_operand,
            .RSHIFT => result = left_operand >> shift_operand,
        }
        self.value = result;
        self.needs_evaluation = false;

        //std.debug.print("evaluate \"{s}\": \"{s}\" {s} \"{s}\" = {d}\n", .{ self.id, left_id, @tagName(self.op), right_id, self.value });
        return result;
    }
};

pub fn solve() !void {
    var gpa: std.heap.GeneralPurposeAllocator(.{}) = .init;
    const alloc = gpa.allocator();
    defer _ = gpa.deinit();

    var gates_hash: std.StringHashMap(usize) = .init(alloc);
    defer gates_hash.deinit();
    var gates: std.ArrayList(Gate) = try .initCapacity(alloc, 500);
    defer gates.deinit(alloc);
    var ids: std.ArrayList([]const u8) = try .initCapacity(alloc, 500);
    defer ids.deinit(alloc);

    var it = std.mem.tokenizeScalar(u8, input, '\n');
    while (it.next()) |line| {
        var lr_it = std.mem.tokenizeSequence(u8, line, "->");
        const left = lr_it.next().?;
        const right = lr_it.next().?;

        var left_it = std.mem.tokenizeScalar(u8, left, ' ');
        var right_it = std.mem.tokenizeScalar(u8, right, ' ');

        var left_count: u32 = 0;
        while (left_it.next()) |_| left_count += 1;
        left_it.reset();

        switch (left_count) {
            1 => {
                const left_gate = try Gate.get_or_make_gate(alloc, &gates, &gates_hash, &ids, left_it.next().?);
                const right_gate = try Gate.get_or_make_gate(alloc, &gates, &gates_hash, &ids, right_it.next().?);
                gates.items[right_gate].in1 = left_gate;
            },
            2 => {
                _ = left_it.next().?; // "NOT"
                const left_gate = try Gate.get_or_make_gate(alloc, &gates, &gates_hash, &ids, left_it.next().?);
                const right_gate = try Gate.get_or_make_gate(alloc, &gates, &gates_hash, &ids, right_it.next().?);
                gates.items[right_gate].in1 = left_gate;
                gates.items[right_gate].op = .NOT;
            },
            3 => {
                const left_gate1 = try Gate.get_or_make_gate(alloc, &gates, &gates_hash, &ids, left_it.next().?);
                const op = left_it.next().?;
                const left_gate2 = try Gate.get_or_make_gate(alloc, &gates, &gates_hash, &ids, left_it.next().?);
                const right_gate = try Gate.get_or_make_gate(alloc, &gates, &gates_hash, &ids, right_it.next().?);
                gates.items[right_gate].in1 = left_gate1;
                gates.items[right_gate].in2 = left_gate2;
                gates.items[right_gate].op = std.meta.stringToEnum(Gate.OpType, op).?;
            },
            else => unreachable,
        }
    }

    const part1 = gates.items[gates_hash.get("a").?].evaluate(gates);

    for (gates.items, 0..) |_, i| gates.items[i].needs_evaluation = true;
    const b_index = gates_hash.get("b").?;
    gates.items[b_index].value = part1;
    gates.items[b_index].needs_evaluation = false;
    const part2 = gates.items[gates_hash.get("a").?].evaluate(gates);

    std.debug.print("{d}\n{d}\n", .{ part1, part2 });
}
