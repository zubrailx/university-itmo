import { createSignal } from "solid-js";

import Form, { FieldMeta, FormData } from "~/components/auth/form";


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
  { label: "Password", required: true, key: FormFieldKeys.PASSWORD },
  { label: "Name", required: true, key: FormFieldKeys.NAME },
  { label: "Description", required: false, key: FormFieldKeys.DESCRIPTION },
  { label: "Address", required: false, key: FormFieldKeys.ADDRESS },
]

const [asCafe, setAsCafe] = createSignal<boolean>(true)

export default function SignUp() {
  return (
    <Form
      fields={fields}
      data={data}
      setData={setData}
      asCafe={asCafe}
      setAsCafe={setAsCafe}
      buttonText="Sign Up"
    />
  )
}
