pub const packages = struct {
    pub const @"N-V-__8AAHOzAQBh8wB371GN1DXTl1mKs8Rdqj0sJea0U4P7" = struct {
        pub const build_root = "/home/jaipal001/.cache/zig/p/N-V-__8AAHOzAQBh8wB371GN1DXTl1mKs8Rdqj0sJea0U4P7";
        pub const build_zig = @import("N-V-__8AAHOzAQBh8wB371GN1DXTl1mKs8Rdqj0sJea0U4P7");
        pub const deps: []const struct { []const u8, []const u8 } = &.{};
    };
    pub const @"httpz-0.0.0-PNVzrOu4BgCBCARbwWC7MUn7_MqnGwUNfTXEUryOns06" = struct {
        pub const build_root = "/home/jaipal001/.cache/zig/p/httpz-0.0.0-PNVzrOu4BgCBCARbwWC7MUn7_MqnGwUNfTXEUryOns06";
        pub const build_zig = @import("httpz-0.0.0-PNVzrOu4BgCBCARbwWC7MUn7_MqnGwUNfTXEUryOns06");
        pub const deps: []const struct { []const u8, []const u8 } = &.{
            .{ "metrics", "N-V-__8AAHOzAQBh8wB371GN1DXTl1mKs8Rdqj0sJea0U4P7" },
            .{ "websocket", "websocket-0.1.0-ZPISdXNIAwCXG7oHBj4zc1CfmZcDeyR6hfTEOo8_YI4r" },
        };
    };
    pub const @"websocket-0.1.0-ZPISdXNIAwCXG7oHBj4zc1CfmZcDeyR6hfTEOo8_YI4r" = struct {
        pub const build_root = "/home/jaipal001/.cache/zig/p/websocket-0.1.0-ZPISdXNIAwCXG7oHBj4zc1CfmZcDeyR6hfTEOo8_YI4r";
        pub const build_zig = @import("websocket-0.1.0-ZPISdXNIAwCXG7oHBj4zc1CfmZcDeyR6hfTEOo8_YI4r");
        pub const deps: []const struct { []const u8, []const u8 } = &.{
        };
    };
};

pub const root_deps: []const struct { []const u8, []const u8 } = &.{
    .{ "httpz", "httpz-0.0.0-PNVzrOu4BgCBCARbwWC7MUn7_MqnGwUNfTXEUryOns06" },
};
