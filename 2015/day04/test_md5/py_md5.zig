const std = @import("std");
const c = @cImport({
    @cInclude("cpython/md5.h");
});
pub fn main() !void {
    var out: [std.crypto.hash.Md5.digest_length]u8 = undefined;
    for (0..10_000_000) |_| {
        _ = c.MD5("yzbqklnj", 8, &out);
        //const out_hex = std.fmt.bytesToHex(out, std.fmt.Case.lower);
        //std.debug.print("c {s}", .{out_hex});
    }
}
