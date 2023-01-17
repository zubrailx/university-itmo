import { createSignal } from 'solid-js';

export type UserData = {
  token?: string
}

export const [userData, setUserData] = createSignal<UserData>({} as UserData)
