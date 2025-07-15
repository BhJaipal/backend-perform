export interface MessageProps {
	replyTo?: Message | null;
	type: "image" | "text";
	sentByYou: boolean;
}
export interface Message {
	name: string;
	msg: string;
	prop: MessageProps;
}

export class Ref<T> {
	_value: Array<T>;
	_target: string;
	onFn?: (key: number | string, value: T) => HTMLElement;

	constructor(target: string, value: Array<T>) {
		this._target = target;
		this._value = value;
	}

	get getConst() {
		const val = this._value;
		return val satisfies readonly T[];
	}
	on(fn: (key: number | string, value: T) => HTMLElement) {
		this.onFn = fn;
		return this._value.map((value, index) => {
			return fn(index, value);
		});
	}
	push(value: T, onUpdate = Ref.updateOnPush) {
		this._value.push(value);
		onUpdate(this, value);
	}
	pop(
		onUpdate = Ref.updateOnPush,
		additionalEffs: ((_: T) => {}) | undefined = undefined
	) {
		let last = this._value.pop();
		if (last) {
			onUpdate(this, last);
			if (additionalEffs) additionalEffs(last);
		}
		return last;
	}
	map(fn: (value: T, index: number) => HTMLElement) {
		return this._value.map(fn);
	}
	set value(value: Array<T>) {
		this._value = value;
		Ref.rebuild(this);
	}

	static updateOnPop<T>(ref: Ref<T>, value: T) {
		let target = get(ref._target);
		if (!target) {
			console.error(`Element with selector ${ref._target} not found`);
			return;
		}
		if (ref.onFn)
			target.removeChild(ref.onFn(ref._value.length - 1, value));
	}
	static updateOnPush<T>(ref: Ref<T>, value: T) {
		let target = get(ref._target);
		if (!target) {
			console.error(`Element with selector ${ref._target} not found`);
			return;
		}
		if (ref.onFn)
			target.appendChild(ref.onFn(ref._value.length - 1, value));
	}
	static rebuild<T>(ref: Ref<T>) {
		if (!ref.onFn) {
			console.error("onFn is not defined, can't rebuild state");
			return;
		}
		let target = get(ref._target);
		if (!target) {
			console.error(
				"Element with selector '" + ref._target + "' not found"
			);
			return;
		}
		let fn = ref.onFn;
		if (fn) {
			target.innerHTML = "";
			target.append(
				...ref._value.map((value, index) => {
					console.error("onFn is not defined, can't rebuild state");
					return fn(index, value);
				})
			);
		}
	}
}

export type allowedKeys = "ctrl" | "alt";

export let map: Array<{
	mods: Array<allowedKeys> | allowedKeys;
	key: string;
	event: (e: KeyboardEvent) => void;
}> = [];

export class Mount {
	static selector: string;
	static el: HTMLElement | null = null;
	constructor(selector: string, el: HTMLElement) {
		if (Mount.selector == selector) {
			console.error("An Element is already mounted, Do not mount again");
			return;
		}
		const root = document.querySelector(selector);
		if (!root) {
			console.error("Element with selector '" + selector + "' not found");
			return;
		}
		root.appendChild(el);
		Mount.selector = selector;
		Mount.el = el;
	}
}
export function mount(selector: string, el: HTMLElement) {
	return new Mount(selector, el);
}

export function attachKeyBind(
	mods: Array<allowedKeys> | allowedKeys,
	key: string,
	event: (e: KeyboardEvent) => void
) {
	map.push({ mods, key, event });
}

export function get<T extends HTMLElement>(selector: string): T | null {
	// @ts-ignore: Unsafe TypeScript assertion
	return document.querySelector<T>(selector);
}
export function getAll<T extends HTMLElement>(
	selector: string
): Array<T | null> {
	// @ts-ignore: Unsafe TypeScript assertion
	return document.querySelectorAll<T>(selector);
}
