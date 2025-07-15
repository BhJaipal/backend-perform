export interface ElementProps {
	id?: string;
	class?: Array<string> | string;
	text?: string;
	style?: Partial<CSSStyleDeclaration>;
}
export type elementsTag =
	| "a"
	| "abbr"
	| "button"
	| "code"
	| "em"
	| "i"
	| "span"
	| "h1"
	| "h2"
	| "h3"
	| "h4"
	| "h5"
	| "h6"
	| "input"
	| "textarea"
	| "div"
	| "p"
	| "kbd"
	| "img";

export function makeElement(
	tag: elementsTag,
	html: HTMLElement | string | Array<HTMLElement>,
	props: ElementProps = {}
): HTMLElement {
	let btn = document.createElement(tag);
	if (props.id) btn.id = props.id;
	if (props.class) {
		if (Array.isArray(props.class)) btn.classList.add(...props.class);
		else btn.className = props.class;
	}
	if (html) {
		if (typeof html === "string") btn.innerHTML = html;
		else if (html instanceof Array) btn.append(...html);
		else btn.append(html);
	}
	if (props.text) btn.textContent = props.text;
	if (props.style) Object.assign(btn.style, props.style);
	return btn;
}

export type ButtonProps = ElementProps & {
	onClick?: () => void;
	disabled?: boolean;
};

export function Button(
	html: HTMLElement | string | Array<HTMLElement>,
	props: ButtonProps = {}
) {
	let btn = makeElement("button", html, props) as HTMLButtonElement;
	if (props.onClick) btn.addEventListener("click", props.onClick);
	if (props.disabled) btn.disabled = props.disabled;
	return btn;
}

export function Icon(name: string, props: ElementProps = {}) {
	let icon = makeElement("i", "", props);
	icon.classList.add("material-icons");
	icon.textContent = name;
	return icon;
}

export function HTag(
	level: number,
	html: HTMLElement | string | Array<HTMLElement>,
	props: ElementProps = {}
) {
	let slevel: elementsTag;
	if (level > 6) slevel = "h6";
	else slevel = "h1";
	switch (level) {
		case 1:
			slevel = "h1";
			break;
		case 2:
			slevel = "h2";
			break;
		case 3:
			slevel = "h3";
			break;
		case 4:
			slevel = "h4";
			break;
		case 5:
			slevel = "h5";
			break;
		case 6:
			slevel = "h6";
			break;
	}

	return makeElement(slevel, html, props);
}
export type InputProps = ElementProps & {
	placeholder?: string;
	type?: string;
};

export function Input(
	html: HTMLElement | string | Array<HTMLElement>,
	props: InputProps = {}
) {
	let btn = makeElement("input", html, props) as HTMLInputElement;
	if (props.style) Object.assign(btn.style, props.style);
	if (props.placeholder) btn.placeholder = props.placeholder;
	if (props.type) btn.type = props.type;
	return btn;
}
export function Textarea(
	html: HTMLElement | string | Array<HTMLElement>,
	props: ElementProps & { placeholder?: string } = {}
) {
	let btn = makeElement("textarea", html, props) as HTMLTextAreaElement;
	if (props.style) Object.assign(btn.style, props.style);
	if (props.placeholder) btn.placeholder = props.placeholder;
	return btn;
}
export function Kbd(
	html: HTMLElement | string | Array<HTMLElement>,
	props: ElementProps = {}
) {
	return makeElement("kbd", html, props);
}

export function Column(
	html: HTMLElement | string | Array<HTMLElement>,
	props: ElementProps & { reverse?: boolean } = {}
) {
	let el = makeElement("div", html, props);
	el.style.display = "flex";
	if (props.reverse) el.style.flexDirection = "column-reverse";
	else el.style.flexDirection = "column";
	return el;
}

export function Row(
	html: HTMLElement | string | Array<HTMLElement>,
	props: ElementProps & { reverse?: boolean } = {}
) {
	let el = makeElement("div", html, props);
	el.style.display = "flex";
	if (props.reverse) el.style.flexDirection = "row-reverse";
	else el.style.flexDirection = "row";
	return el;
}
export function Box(
	html: HTMLElement | string | Array<HTMLElement>,
	props: ElementProps = {}
) {
	let el = makeElement("div", html, props) as HTMLDivElement;
	return el;
}

export function Img(src: string, props: ElementProps & { alt?: string } = {}) {
	let el = makeElement("img", "", props) as HTMLImageElement;
	if (props.alt) el.alt = props.alt;
	el.src = src;
	return el;
}
