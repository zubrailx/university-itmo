import { createSignal } from "solid-js";

import Form, { FieldMeta, FormData } from "~/components/auth/form";
import { serverURL } from "~/data/fetcher";
import { setUserData, userData } from "~/data/user-store";
import { setServerError } from "./data";


enum FormFieldKeys {
  USERNAME = "username",
  PASSWORD = "password",
}

const [data, setData] = createSignal<FormData<FormFieldKeys>>({} as FormData<FormFieldKeys>)

const fields: FieldMeta<FormFieldKeys>[] = [
  { label: "Username", required: true, key: FormFieldKeys.USERNAME },
  { label: "Password", required: true, key: FormFieldKeys.PASSWORD, type: "password" },
]

export default function SignIn() {
  return (
    <Form
      fields={fields}
      data={data}
      setData={setData}
      buttonText="Sign In"
      clearServerError={() => setServerError(undefined)}
      onSuccess={(body) => {
        fetch(
          `${serverURL}/sign-in/`, {
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
            setData({
              ...data(),
              username: { ...data().username, error: true },
              password: { ...data().password, error: true },
            })
            setServerError("Wrong credentials")
          }
          else res.text().then(text => console.log(res.status, text))
        })
      }}
    />
  )
}
