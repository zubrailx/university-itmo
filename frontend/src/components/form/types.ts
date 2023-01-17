import { PropsOf } from "@suid/types";

export type FieldValue = { value?: string, error?: boolean }
export type FormData<K extends string> = { [key in K]: FieldValue }

export type FieldMeta<K extends string> = {
    key: K,
    label: string,
    required: boolean,
    type?: PropsOf<"input">["type"],
}
