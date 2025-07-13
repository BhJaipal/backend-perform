import "./style.css";

import { Box, Button, Column, Icon, Img, Input, Kbd, Row } from "./dom";

type allowedKeys = "ctrl" | "alt";

let map: Array<{
	mods: Array<allowedKeys> | allowedKeys;
	key: string;
	event: () => void;
}> = [];

class MountElement {
	static selector: string;
	static el: HTMLElement | null = null;
	constructor(selector: string, el: HTMLElement) {
		if (MountElement.selector == selector) {
			console.error("An Element is already mounted, Do not mount again");
			return;
		}
		const root = document.querySelector(selector);
		if (!root) {
			console.error("Element with selector '" + selector + "' not found");
			return;
		}
		root.appendChild(el);
		MountElement.selector = selector;
		MountElement.el = el;
	}
}

function mount(selector: string, el: HTMLElement) {
	new MountElement(selector, el);
}

interface MessageProps {
	replyTo?: Message | null;
	type: "image" | "text";
	sentByYou: boolean;
}
interface Message {
	name: string;
	msg: string;
	prop: MessageProps;
}

let chats: Array<Message> = [
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
];

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
			e.preventDefault();
			e.stopPropagation();
			event();
		}
	});
});

function attachKeyBind(
	mods: Array<allowedKeys> | allowedKeys,
	key: string,
	event: () => void
) {
	map.push({ mods, key, event });
}

function get<T extends HTMLElement>(selector: string): T | null {
	// @ts-ignore: Unsafe TypeScript assertion
	return document.querySelector<T>(selector);
}

attachKeyBind([], "Escape", () => {
	get<HTMLDivElement>("#chat-input")?.blur();
});
attachKeyBind([], "i", () => {
	get<HTMLInputElement>("#chat-input")?.focus();
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

let currIndex = 0;

// Scroll Up the chats
attachKeyBind([], "k", () => {
	if (currIndex == chats.length - 1) {
		return;
	}
	currIndex++;
	document
		.querySelectorAll<HTMLDivElement>(".message")
		[chats.length - currIndex - 1].classList.add("focus");
	document
		.querySelectorAll<HTMLDivElement>(".message")
		[chats.length - currIndex].classList.remove("focus");
});
attachKeyBind([], "ArrowUp", () => {
	if (currIndex == chats.length - 1) {
		return;
	}
	currIndex++;
	document
		.querySelectorAll<HTMLDivElement>(".message")
		[chats.length - currIndex - 1].classList.add("focus");
	document
		.querySelectorAll<HTMLDivElement>(".message")
		[chats.length - currIndex].classList.remove("focus");
});

// Scroll down the chats
attachKeyBind([], "j", () => {
	if (!currIndex) {
		return;
	}
	currIndex--;
	document
		.querySelectorAll<HTMLDivElement>(".message")
		[chats.length - currIndex - 2].classList.remove("focus");
	document
		.querySelectorAll<HTMLDivElement>(".message")
		[chats.length - currIndex - 1].classList.add("focus");
});
attachKeyBind([], "ArrowDown", () => {
	if (!currIndex) {
		return;
	}
	currIndex--;
	document
		.querySelectorAll<HTMLDivElement>(".message")
		[chats.length - currIndex - 2].classList.remove("focus");
	document
		.querySelectorAll<HTMLDivElement>(".message")
		[chats.length - currIndex - 1].classList.add("focus");
});

mount(
	"#app",
	Box(
		Box([
			Column(
				chats.map((msg) => message(msg.name, msg.msg, msg.prop)),
				{
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
					}),
				],
				{
					style: { marginTop: "50px" },
				}
			),
		])
	)
);

get<HTMLInputElement>("#chat-input")?.addEventListener("keydown", (_) => {
	let button = get<HTMLButtonElement>("#chat-send");
	if (!button) return;
	if (get<HTMLInputElement>("#chat-input")?.value.trim()) {
		button.disabled = false;
	} else {
		button.disabled = true;
	}
});
