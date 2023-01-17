import { Navigator, useNavigate } from "@solidjs/router";
import { createSignal } from "solid-js";

import { convertData } from "~/components/form/data";
import Form from "~/components/form/form";
import { FieldMeta, FormData } from "~/components/form/types";
import { serverURL } from "~/data/fetcher";
import { saveToken } from "~/data/user-store";
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

function sendRequest(body: any, navigator: Navigator) {
  fetch(
    `${serverURL}/sign-in/`, {
    method: "POST",
    credentials: "include",
    cache: "no-cache",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify(body),
  }).then(res => {
    if (res.ok) res.json().then(data => {
      saveToken(data.access_token)
      // navigator
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
}

export default function SignIn() {
  const navigator = useNavigate()

  return (
    <Form
      fields={fields}
      data={data}
      setData={setData}
      buttonText="Sign In"
      clearServerError={() => setServerError(undefined)}
      onSuccess={() => sendRequest(convertData(data()), navigator)}
    />
  )
}
