import { createSignal } from "solid-js";

import Form, { FieldMeta, FormData } from "~/components/auth/form";
import { serverURL } from "~/data/fetcher";
import { setUserData, userData } from "~/data/user-store";


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
const [serverError, setServerError] = createSignal<string | undefined>(undefined)

export default function SignUp() {
  return (
    <Form
      fields={fields}
      data={data}
      setData={setData}
      asCafe={asCafe}
      setAsCafe={setAsCafe}
      serverError={serverError()}
      clearServerError={() => setServerError(undefined)}
      buttonText="Sign Up"
      onSuccess={(data) => {
        fetch(
          `${serverURL}/sign-up/`, {
          method: "POST",
          credentials: "include",
          cache: "no-cache",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify(data)
        }).then(res => {
          if (res.ok) res.json().then(data => {
            setUserData({ ...userData, token: data.access_token })
          })
          else if (res.status === 401) setServerError("Username taken")
          else res.text().then(text => console.log(res.status, text))
        })
      }}
    />
  )
}
