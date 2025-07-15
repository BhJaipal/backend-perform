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

export function defaultOnIf(value: string | number | boolean) {
	if (typeof value === "string") value = value.trim();
	return !!value;
}

export class React<T extends number | string | boolean> {
	private _value: T;
	onFn: Array<[(value: T) => unknown, number]>;
	private onIf: (value: T) => boolean;

	constructor(value: T, onIf: (value: T) => boolean = defaultOnIf) {
		this._value = value;
		this.onFn = [];
		this.onIf = onIf;
	}
	set value(value: T) {
		this._value = value;
		React.rebuild(this);
	}
	on(fn: (value: T) => unknown) {
		this.onFn?.push([fn, -1]);
		React.rebuild(this);
	}
	onTrue(fn: (value: T) => unknown) {
		this.onFn.push([fn, 1]);
		if (this._value) {
			fn(this._value);
		}
	}
	onFalse(fn: (value: T) => unknown) {
		let f2 = (value: T) => {
			if (!value) {
				fn(value);
			}
		};
		this.onFn.push([fn, 0]);
		f2(this._value);
	}
	static rebuild<T extends boolean | number | string>(ref: React<T>) {
		ref.onFn.forEach(([fn, i]) => {
			if (i == -1) fn(ref._value);
			else if (i && ref.onIf(ref._value)) fn(ref._value);
			else if (!i && !ref.onIf(ref._value)) fn(ref._value);
		});
	}
}

export type allowedKeys = "ctrl" | "alt";

export let map: Array<{
	mods: Array<allowedKeys> | allowedKeys;
	key: string;
	event: (e: KeyboardEvent) => void;
}> = [];

export class Mount {
	static mounted: Map<string, HTMLElement> = new Map();

	constructor(selector: string, el: HTMLElement) {
		if (Mount.mounted.has(selector)) {
			console.error("An Element is already mounted, Do not mount again");
			return;
		}
		const root = document.querySelector(selector);
		if (!root) {
			console.error("Element with selector '" + selector + "' not found");
			return;
		}
		root.appendChild(el);
		Mount.mounted.set(selector, el);
	}
}
export function remount(selector: string, el: HTMLElement) {
	let target = get(selector);
	if (!target) {
		console.error(`Element with selector ${selector} not found`);
		return;
	}
	if (!Mount.mounted.has(selector)) {
		console.warn("Element with selector '" + selector + "' not mounted");
	}
	target.innerHTML = "";
	target.appendChild(el);
	Mount.mounted.set(selector, el);
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

export function detachKeyBind(
	mods: Array<allowedKeys> | allowedKeys,
	key: string
) {
	map = map.filter((m) => !(m.mods === mods && m.key === key));
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
