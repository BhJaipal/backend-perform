const std = @import("std");
const httpz = @import("httpz");

pub const User = struct {
    name: []u8,
    id: u8,
    password: []u8,
    token: [64]u8,
    pub fn init(name: []u8, passwd: []u8) User {
        const id = gen_id();
        return .{ .name = name, .id = id, .password = passwd, .token = gen_token(id) };
    }
    pub fn send_token(self: *const User, res: *httpz.Response) !void {
        try res.json(.{ .auth = self.token }, .{});
    }
};

fn gen_token(id: u8) [64]u8 {
    const hash = std.crypto.hash;
    var sha256 = hash.sha2.Sha256.init(.{});
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    const allocator = gpa.allocator();
    const out: [64]u8 = ("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF").*;
    const src = std.fmt.allocPrint(allocator, "{}", .{id}) catch |err| switch (err) {
        std.fmt.AllocPrintError.OutOfMemory => {
            std.log.err("Cannot convert u8 to string\n", .{});
            return out;
        },
    };
    sha256.update(src);

    const result = sha256.finalResult();
    const hex = std.fmt.bytesToHex(result, .lower);
    return hex;
}
fn gen_id() u8 {
    const rand = std.crypto.random;
    return rand.int(u8);
}

pub const Message = struct {
    author: []u8,
    msg: []u8,
    token: [64]u8,
    timestamp: struct { hr: u8, min: u8 },
    pub fn init(author: []u8, msg: []u8, token: [64]u8, hr: u8, min: u8) Message {
        return .{ .author = author, .msg = msg, .token = token, .timestamp = .{ .hr = hr, .min = min } };
    }
    pub fn to_json(self: *const Message, res: *httpz.Response) !void {
        try res.json(.{ .author = self.author, .msg = self.msg, .timestamp = .{ .hr = self.timestamp.hr, .min = self.timestamp.min } }, .{});
    }
};
