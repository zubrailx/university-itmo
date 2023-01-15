import { Button, Stack, TextField, ToggleButton, ToggleButtonGroup, Typography } from "@suid/material";
import { Accessor, For, Setter } from "solid-js";

export type FieldValue = { value?: string, error?: boolean }
export type FormData<K extends string> = { [key in K]: FieldValue }

export type FieldMeta<K extends string> = {
  key: K,
  label: string,
  required: boolean,
}

export type FormProps<K extends string> = {
  fields: FieldMeta<K>[],
  data: Accessor<FormData<K>>,
  setData: Setter<FormData<K>>,
  asCafe?: Accessor<boolean>,
  setAsCafe?: Setter<boolean>,
  buttonText: string,
  serverError?: string,
  clearServerError: () => void,
  onSuccess: (data: any) => void,
}

export default function Form<K extends string>(props: FormProps<K>) {
  return (
    <Stack spacing="16px">
      <For each={props.fields}>
        {
          ({ key, label }) => <>
            <TextField
              label={label}
              variant="outlined"
              value={props.data()[key]?.value || ""}
              error={props.data()[key]?.error}
              onChange={(event: any) => {
                props.clearServerError()
                props.setData(
                  (prev) => ({ ...prev, [key]: { value: event.target.value } } as FormData<K>)
                )
              }}
            />
          </>
        }
      </For>
      {props.asCafe !== undefined
        && <ToggleButtonGroup
          color="primary"
          value={props.asCafe()}
          onChange={(_, value: boolean) => props.setAsCafe && props.setAsCafe(() => value)}
          fullWidth
          exclusive
        >
          <ToggleButton value={true}>
            Cafe
          </ToggleButton>
          <ToggleButton value={false}>
            Supplier
          </ToggleButton>
        </ToggleButtonGroup>
      }
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
          props.onSuccess(
            Object.fromEntries(
              (Object.entries<FieldValue>(props.data())
                .map(([key, { value }]) => [key, value]) as [string, string | boolean | undefined][])
                .concat(props.asCafe === undefined ? [] : [["as_cafe", props.asCafe()]])
                .filter(([_, value]) => !!value)
            )
          )
        }}
      >
        {props.buttonText}
      </Button>
      {props.serverError !== undefined
        && <Typography
          variant="body1"
          color="error"
        >
          {props.serverError}
        </Typography>
      }
    </Stack>
  )
}
