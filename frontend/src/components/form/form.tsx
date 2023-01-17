import { Button, Stack, TextField } from "@suid/material";
import { Accessor, For, JSXElement, Setter } from "solid-js";
import { FieldMeta, FieldValue, FormData } from "./types";

export type FormProps<K extends string> = {
  fields: FieldMeta<K>[],
  data: FormData<K>,
  setData: (data: FormData<K>) => void,
  buttonText: string,
  clearServerError: () => void,
  errorCheck?: () => boolean,
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
              value={props.data[key]?.value || ""}
              error={props.data[key]?.error}
              onChange={({ target: { value: rawValue } }: any) => {
                const value: string = type === "number" ? rawValue.replaceAll(/[^\-0-9]/g, "").substring(0, 10) : rawValue
                props.clearServerError()
                props.setData(({ ...props.data, [key]: { value } } as FormData<K>))
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
            Object.fromEntries(
              props.fields.map(
                ({ key, required }) =>
                  [key, { error: required && !!!props.data[key]?.value, ...props.data[key] }]
              )
            ) as FormData<K>
          )

          const primaryError = Object.values<FieldValue>(props.data).some(({ error }) => error)
          const additionalError = !!props.errorCheck && props.errorCheck()
          if (primaryError || additionalError) return

          props.onSuccess()
        }}
      >
        {props.buttonText}
      </Button>
    </Stack>
  )
}
