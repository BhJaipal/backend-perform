import "./style.css";

import {
	Box,
	Button,
	Column,
	HTag,
	Icon,
	Img,
	Input,
	Kbd,
	Label,
	Row,
} from "./dom";
import {
	attachKeyBind,
	detachKeyBind,
	get,
	getAll,
	map,
	mount,
	React,
	Ref,
	remount,
	type Message,
	type MessageProps,
} from "./update";

let chats = new Ref<Message>("#chats-list", [
	{
		name: "jaipal",
		msg: "Hello World",
		prop: { sentByYou: true, type: "text" },
	},
	{
		name: "sahil",
		msg: "Error 404",
		prop: { sentByYou: false, type: "text" },
	},
]);

document.addEventListener("keydown", (e) => {
	map.forEach(({ mods, key, event }) => {
		let mod = Array.isArray(mods) ? mods : [mods];
		for (let i = 0; i < mod.length; i++) {
			if (mods.length == 0) {
				if (e.ctrlKey || e.altKey || e.metaKey) {
					return;
				}
			} else if (mod[i] == "ctrl" && !e.ctrlKey) return;
			else if (mod[i] == "alt" && !e.altKey) return;
		}
		if (key == e.key) {
			event(e);
		}
	});
});

function messageData(message: string, props: MessageProps) {
	return props.type == "text"
		? Box(message, {
				style: {
					fontSize: "larger",
				},
		  })
		: Img(message, {
				alt: message.split(".")[0],
				style: {
					width: "40vw",
					border: "2px solid lightgreen",
				},
		  });
}

function message(author: string, message: string, props: MessageProps) {
	if (props.sentByYou) {
		return Box(
			Box(
				[
					Box("You", {
						style: {
							fontWeight: "bold",
							textAlign: "end",
						},
						class: ["name"],
					}),
					messageData(message, props),
				],
				{ style: { textAlign: "start" } }
			),
			{
				class: ["message", "sender"],
				style: {
					display: "flex",
					justifyContent: "end",
					paddingRight: "20px",
				},
			}
		);
	} else {
		return Box(
			Row(
				[
					Img("/user.png", {
						class: ["user-pfp"],
					}),
					Box(
						Box(
							[
								Box(author, {
									style: {
										fontWeight: "bold",
									},
									class: "name",
								}),
								messageData(message, props),
							],
							{
								style: {
									textAlign: "start",
								},
							}
						)
					),
				],
				{
					style: {
						columnGap: "20px",
						height: "min-content",
					},
				}
			),
			{
				class: ["message"],
				style: {
					display: "flex",
					justifyContent: "start",
					paddingLeft: "20px",
				},
			}
		);
	}
}

let currIndex = -1;

function scrollUp() {
	if (currIndex >= chats.getConst.length - 1) {
		return;
	}
	if (currIndex == -1) {
		document
			.querySelectorAll<HTMLDivElement>(".message")
			[chats.getConst.length - 1].classList.add("focus");
		currIndex++;
		return;
	}
	currIndex++;
	document
		.querySelectorAll<HTMLDivElement>(".message")
		[chats.getConst.length - currIndex - 1].classList.add("focus");
	document
		.querySelectorAll<HTMLDivElement>(".message")
		[chats.getConst.length - currIndex].classList.remove("focus");
}
function scrollDown() {
	if (!currIndex) {
		currIndex = -1;
	}
	if (currIndex == -1) {
		document
			.querySelectorAll<HTMLDivElement>(".message")
			[chats.getConst.length - 1].classList.remove("focus");
		return;
	}
	currIndex--;
	document
		.querySelectorAll<HTMLDivElement>(".message")
		[chats.getConst.length - currIndex - 2].classList.remove("focus");
	document
		.querySelectorAll<HTMLDivElement>(".message")
		[chats.getConst.length - currIndex - 1].classList.add("focus");
}

let loggedIn = new React<boolean>(false);

loggedIn.onTrue((_) => {
	// Scroll Up the chats
	attachKeyBind([], "k", () => {
		if (get<HTMLInputElement>("#chat-input") == document.activeElement)
			return;
		scrollUp();
	});
	attachKeyBind([], "ArrowUp", () => {
		scrollUp();
	});

	// Scroll down the chats
	attachKeyBind([], "j", () => {
		if (get<HTMLInputElement>("#chat-input") == document.activeElement)
			return;
		scrollDown();
	});
	attachKeyBind([], "ArrowDown", () => {
		scrollDown();
	});
	get<HTMLInputElement>("#chat-input")?.addEventListener("keydown", (_) => {
		let button = get<HTMLButtonElement>("#chat-send");
		if (!button) return;
		if (get<HTMLInputElement>("#chat-input")?.value.trim()) {
			button.disabled = false;
		} else {
			button.disabled = true;
		}
	});

	attachKeyBind([], "Enter", () => {
		let button = get<HTMLButtonElement>("#chat-send");
		if (!button) return;
		button.click();
	});
	attachKeyBind([], "Escape", (e) => {
		e.preventDefault();
		get<HTMLDivElement>("#chat-input")?.blur();
		if (currIndex != -1) {
			getAll(".message")[
				chats.getConst.length - currIndex - 1
			]?.classList.remove("focus");
			currIndex = -1;
		}
	});
	attachKeyBind([], "i", (e: KeyboardEvent) => {
		if (get<HTMLInputElement>("#chat-input") == document.activeElement)
			return;
		e.preventDefault();
		get<HTMLInputElement>("#chat-input")?.focus();
		if (currIndex != -1) {
			getAll(".message")[
				chats.getConst.length - currIndex - 1
			]?.classList.remove("focus");
			currIndex = -1;
		}
	});
});
loggedIn.onFalse((_) => {
	detachKeyBind([], "k");
	detachKeyBind([], "j");
	detachKeyBind([], "ArrowUp");
	detachKeyBind([], "ArrowDown");
	detachKeyBind([], "Enter");
	detachKeyBind([], "i");
	detachKeyBind([], "Escape");
});

loggedIn.onTrue((_) =>
	remount(
		"#app",
		Box(
			Box([
				Column(
					chats.on((_, msg) => message(msg.name, msg.msg, msg.prop)),
					{
						id: "chats-list",
						style: {
							height: "80vh",
							rowGap: "20px",
							overflowY: "scroll",
							alignItems: "center",
						},
					}
				),
				Row(
					[
						Input("", {
							id: "chat-input",
						}),
						Box(
							Kbd("i", {
								id: "chat-input-shotcut",
							})
						),
						Button(Icon("send"), {
							id: "chat-send",
							disabled: true,
							onClick() {
								let input =
									get<HTMLInputElement>("#chat-input");
								if (!input) return;

								chats.push({
									name: "You",
									msg: input.value,
									prop: { sentByYou: true, type: "text" },
								});
								input.value = "";
								this.disabled = true;
							},
						}),
					],
					{
						style: { marginTop: "50px" },
					}
				),
			]),
			{
				style: {
					display: "flex",
					justifyContent: "center",
				},
			}
		)
	)
);

mount(
	"#app",
	Box(
		Column(
			[
				Box(
					HTag(1, "Login", {
						style: {
							color: "#9eedfdff",
							textShadow: "0 0 1rem #11d3f9",
						},
					}),
					{
						style: { display: "flex", justifyContent: "center" },
					}
				),
				Row(
					[
						Label("Name: ", {
							htmlFor: "login-name",
							style: {
								display: "flex",
								alignItems: "center",
								fontSize: "20px",
							},
						}),
						Input("", {
							id: "login-name",
							style: { width: "500px" },
						}),
					],
					{
						style: {
							padding: "10px 0",
							justifyContent: "space-evenly",
						},
					}
				),
				Row(
					[
						Label("Password", {
							htmlFor: "login-password",
							style: {
								display: "flex",
								alignItems: "center",
								fontSize: "20px",
							},
						}),
						Input("", {
							type: "password",
							id: "login-password",
							style: { width: "500px" },
						}),
					],
					{
						style: {
							padding: "10px 0",
							justifyContent: "space-evenly",
						},
					}
				),
				Box(
					Button("Submit", {
						onClick: login,
						disabled: true,
						id: "login-button",
					}),
					{
						style: {
							display: "flex",
							justifyContent: "center",
						},
					}
				),
			],
			{ style: { width: "700px" } }
		),
		{
			style: {
				display: "flex",
				justifyContent: "center",
				alignItems: "start",
			},
		}
	)
);

function login() {
	let name = get<HTMLInputElement>("#login-name");
	let password = get<HTMLInputElement>("#login-password");
	if (name && password) {
		console.log(name.value, password.value);
		loggedIn.value = true;
	}
}
loggedIn.on((val) => {
	if (!val) {
		let name = get<HTMLInputElement>("#login-name");
		let password = get<HTMLInputElement>("#login-password");
		if (!name || !password) {
			return;
		}
		let btn = get<HTMLButtonElement>("#login-button");
		if (!btn) return;
		name.onchange = () => {
			btn.disabled = !name.value.trim() || !password.value.trim();
		};
		password.onchange = () => {
			btn.disabled = !name.value.trim() || !password.value.trim();
		};
	}
});
