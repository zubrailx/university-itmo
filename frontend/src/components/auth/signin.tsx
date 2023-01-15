import { createSignal } from "solid-js";

import Form, { FieldMeta, FormData } from "~/components/auth/form";
import { serverURL } from "~/data/fetcher";
import { setUserData, userData } from "~/data/user-store";


enum FormFieldKeys {
  USERNAME = "username",
  PASSWORD = "password",
}

const [data, setData] = createSignal<FormData<FormFieldKeys>>({} as FormData<FormFieldKeys>)

const fields: FieldMeta<FormFieldKeys>[] = [
  { label: "Username", required: true, key: FormFieldKeys.USERNAME },
  { label: "Password", required: true, key: FormFieldKeys.PASSWORD },
]

const [serverError, setServerError] = createSignal<string | undefined>(undefined)

export default function SignIn() {
  return (
    <Form
      fields={fields}
      data={data}
      setData={setData}
      buttonText="Sign In"
      serverError={serverError()}
      clearServerError={() => setServerError(undefined)}
      onSuccess={(data) => {
        console.log(JSON.stringify(data))
        fetch(
          `${serverURL}/sign-in/`, {
          method: "POST",
          credentials: "include",
          cache: "no-cache",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify(data)
        }).then(res => {
          if (res.ok) res.json().then(data => {
            setUserData({ ...userData, token: data.access_token })
          })
          else if (res.status === 401) setServerError("Wrong credentials")
          else res.text().then(text => console.log(res.status, text))
        })
      }}
    />
  )
}
