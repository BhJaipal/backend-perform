const std = @import("std");
const httpz = @import("httpz");

pub fn main() !void {
    std.debug.print("All your {s} are belong to us.\n", .{"codebase"});
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    const allocator = gpa.allocator();
    var server = try httpz.Server(void).init(allocator, .{ .port = 8000 }, {});
    defer {
        server.stop();
        server.deinit();
    }

    var router = try server.router(.{});
    router.get("/", check_msg, .{});

    std.debug.print("Server started at: http://localhost:8000/\n", .{});
    try server.listen();
}

fn check_msg(req: *httpz.Request, res: *httpz.Response) !void {
    std.debug.print("{}\n", .{req.method});
    res.status = 200;
    try res.json(.{ .author = "Jaipal", .msg = "Hello World", .timestamp = .{ .hr = 11, .min = 1 } }, .{});
}
