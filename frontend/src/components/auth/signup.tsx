import { createSignal } from "solid-js";

import Form, { FieldMeta, FormData } from "~/components/auth/form";
import { serverURL } from "~/data/fetcher";
import { setUserData, userData } from "~/data/user-store";
import { setServerError } from "./data";


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
    <Form
      fields={fields}
      data={data}
      setData={setData}
      asCafe={asCafe}
      setAsCafe={setAsCafe}
      clearServerError={() => setServerError(undefined)}
      buttonText="Sign Up"
      onSuccess={(body) => {
        fetch(
          `${serverURL}/sign-up/`, {
          method: "POST",
          credentials: "include",
          cache: "no-cache",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify(body),
        }).then(res => {
          if (res.ok) res.json().then(data => {
            setUserData({ ...userData, token: data.access_token })
          })
          else if (res.status === 401) {
            setServerError("Username taken")
            setData({ ...data(), username: { ...data().username, error: true } })
          }
          else res.text().then(text => console.log(res.status, text))
        })
      }}
    />
  )
}
