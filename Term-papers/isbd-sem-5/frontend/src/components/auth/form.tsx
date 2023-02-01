import { Accessor, JSXElement, Setter } from "solid-js";
import { useNavigate } from "solid-start";
import { serverURL } from "~/data/fetcher";
import { saveToken } from "~/data/user-store";
import { convertData } from "../form/data";
import Form from "../form/form";
import { FieldMeta, FormData } from "../form/types";

export type AuthFormProps<K extends string> = {
  fields: FieldMeta<K>[],
  data: Accessor<FormData<K>>,
  setData: Setter<FormData<K>>,
  buttonText: string,
  clearServerError: () => void,
  errorCheck?: () => boolean,
  children?: JSXElement
  handleError: () => void,
  additions?: [string, string | boolean | undefined][],
  path: string,
  onSuccess?: () => {}
}

export default function AuthForm<K extends string>(props: AuthFormProps<K>) {
  const navigator = useNavigate()

  function sendRequest(body: any) {
    fetch(
      `${serverURL}/${props.path}/`, {
      method: "POST",
      credentials: "include",
      cache: "no-cache",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(body),
    }).then(res => {
      if (res.ok) res.json().then(({ access_token, as_cafe }) => {
        saveToken(access_token)
        navigator(as_cafe ? "/cafes/my" : "/supplier/")
      })
      else if (res.status === 401) props.handleError()
      else res.text().then(text => console.log(res.status, text))
    })
  }

  return <Form
    fields={props.fields}
    data={props.data()}
    setData={props.setData}
    buttonText={props.buttonText}
    clearServerError={props.clearServerError}
    errorCheck={props.errorCheck}
    onSuccess={() => sendRequest(convertData(props.data(), props.additions))}
  >
    {props.children}
  </Form>
}
