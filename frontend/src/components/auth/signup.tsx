import { ToggleButton, ToggleButtonGroup } from "@suid/material";
import { createSignal } from "solid-js";
import Form from "~/components/form/form";
import { FieldMeta, FormData } from "~/components/form/types";
import { serverURL } from "~/data/fetcher";
import { setUserData, userData } from "~/data/user-store";
import { convertData, setServerError } from "./data";


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

function sendRequest(body: any) {
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
}

export default function SignUp() {
  return (
    <Form
      fields={fields}
      data={data}
      setData={setData}
      clearServerError={() => setServerError(undefined)}
      buttonText="Sign Up"
      onSuccess={() => sendRequest(convertData(data(), [["as_cafe", asCafe()]]))}
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
    </Form>
  )
}
