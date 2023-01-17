import { Button, Stack, TextField } from "@suid/material";
import { Accessor, For, JSXElement, Setter } from "solid-js";
import { FieldMeta, FieldValue, FormData } from "./types";

export type FormProps<K extends string> = {
  fields: FieldMeta<K>[],
  data: Accessor<FormData<K>>,
  setData: Setter<FormData<K>>,
  buttonText: string,
  clearServerError: () => void,
  onSuccess: () => void,
  children?: JSXElement
}

export default function Form<K extends string>(props: FormProps<K>) {
  return (
    <Stack spacing="16px">
      <For each={props.fields}>
        {
          ({ key, label, type }) => <>
            <TextField
              label={label}
              variant="outlined"
              type={type === "number" ? "text" : type}
              value={props.data()[key]?.value || ""}
              error={props.data()[key]?.error}
              onChange={({ target: { value: rawValue } }: any) => {
                const value: string = type === "number" ? rawValue.replaceAll(/[^\-0-9]/g, "").substring(0, 10) : rawValue
                props.clearServerError()
                props.setData((prev) => ({ ...prev, [key]: { value } } as FormData<K>))
              }}
            />
          </>
        }
      </For>
      {props.children}
      <Button
        variant="contained"
        onClick={() => {
          props.setData(
            () => Object.fromEntries(
              props.fields.map(
                ({ key, required }) =>
                  [key, { error: required && !!!props.data()[key]?.value, ...props.data()[key] }]
              )
            ) as FormData<K>
          )
          if (Object.values<FieldValue>(props.data()).some(({ error }) => error)) return
          props.onSuccess()
        }}
      >
        {props.buttonText}
      </Button>
    </Stack>
  )
}
