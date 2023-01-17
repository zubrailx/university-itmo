import { createSignal } from "solid-js";
import { FieldMeta, FormData } from "~/components/form/types";
import { setServerError } from "./data";
import AuthForm from "./form";

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
    <AuthForm
      path="sign-in"
      fields={fields}
      data={data}
      setData={setData}
      buttonText="Sign In"
      clearServerError={() => setServerError(undefined)}
      handleError={() => {
        setData({
          ...data(),
          username: { ...data().username, error: true },
          password: { ...data().password, error: true },
        })
        setServerError("Wrong credentials")
      }}
    />
  )
}
