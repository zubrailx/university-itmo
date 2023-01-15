import { createSignal } from "solid-js";

import Form, { FieldMeta, FormData } from "~/components/auth/form";


enum FormFieldKeys {
  USERNAME = "username",
  PASSWORD = "password",
}

const [data, setData] = createSignal<FormData<FormFieldKeys>>({} as FormData<FormFieldKeys>)

const fields: FieldMeta<FormFieldKeys>[] = [
  { label: "Username", required: true, key: FormFieldKeys.USERNAME },
  { label: "Password", required: true, key: FormFieldKeys.PASSWORD },
]

export default function SignIn() {
  return (
    <Form
      fields={fields}
      data={data}
      setData={setData}
      buttonText="Sign In"
    />
  )
}
