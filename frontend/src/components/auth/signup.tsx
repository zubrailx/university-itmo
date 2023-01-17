import { ToggleButton, ToggleButtonGroup } from "@suid/material";
import { createSignal } from "solid-js";
import { FieldMeta, FormData } from "~/components/form/types";
import { setServerError } from "./data";
import AuthForm from "./form";


enum FormFieldKeys {
  USERNAME = "username",
  PASSWORD = "password",
  NAME = "name",
  DESCRIPTION = "description",
  ADDRESS = "address",
}

const [data, setData] = createSignal<FormData<FormFieldKeys>>({} as FormData<FormFieldKeys>)

const fields: FieldMeta<FormFieldKeys>[] = [
  { label: "Username", required: true, key: FormFieldKeys.USERNAME },
  { label: "Password", required: true, key: FormFieldKeys.PASSWORD, type: "password" },
  { label: "Name", required: true, key: FormFieldKeys.NAME },
  { label: "Description", required: false, key: FormFieldKeys.DESCRIPTION },
  { label: "Address", required: false, key: FormFieldKeys.ADDRESS },
]

const [asCafe, setAsCafe] = createSignal<boolean>(true)

export default function SignUp() {
  return (
    <AuthForm
      path="sign-up"
      fields={fields}
      data={data}
      setData={setData}
      clearServerError={() => setServerError(undefined)}
      buttonText="Sign Up"
      additions={[["as_cafe", asCafe()]]}
      handleError={() => {
        setServerError("Username taken")
        setData({ ...data(), username: { ...data().username, error: true } })
      }}
    >
      <ToggleButtonGroup
        color="primary"
        value={asCafe()}
        onChange={(_, value: boolean) => setAsCafe(() => value)}
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
    </AuthForm>
  )
}
