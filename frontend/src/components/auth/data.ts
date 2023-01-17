import { createSignal } from 'solid-js';

export const [serverError, setServerError] = createSignal<string | undefined>(undefined)
