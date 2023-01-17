import { Stack, Typography } from "@suid/material"
import { createEffect, createSignal, JSXElement } from "solid-js"
import { Title, useNavigate, useParams } from "solid-start"
import { serverURL } from "~/data/fetcher"
import { getToken } from "~/data/user-store"
import InnerList, { AnyItem, AnyLink, InnerListProps } from "../common/inner-list"
import { convertData } from "./data"
import Form from "./form"
import { FieldMeta, FormData } from "./types"

export type FormPageProps<K extends string, I extends AnyItem, L extends AnyLink> = {
  path: string,
  formWidth: string,
  listWidth?: string,
  fields: FieldMeta<K>[],
  errorCheck?: () => boolean,
  additions?: [string, string | boolean | number][],
  children?: JSXElement
  unpackData?: (data: any) => void,
  title: string,
  innerList?: InnerListProps<I, L>,
  editOnly?: boolean,
  customId?: string,
}

export default function FormPage<K extends string, I extends AnyItem, L extends AnyLink>(props: FormPageProps<K, I, L>) {
  const [data, setData] = createSignal<FormData<K>>({} as FormData<K>)

  const params = useParams()
  const navigator = useNavigate()

  createEffect(() => {
    if (params.id === "new" && !!!props.editOnly) { }
    else if (params.id === undefined || /^[0-9]+$/g.test(params.id)) {
      fetch(
        `${serverURL}/${props.path}/${params.id ? `${params.id}/` : ""}`, {
        method: "GET",
        credentials: "include",
        headers: { "authorization": `Bearer ${getToken()}` },
      }).then(res => {
        if (res.ok) res.json().then(data => {
          setData(() => Object.fromEntries(
            Object.entries(data).map(([key, value]: any) => ([key, { value: value.toString() }]))
          ) as FormData<K>)
          props.unpackData && props.unpackData(data)
        })
        else if (res.status === 404) navigator("/not-found", { replace: true })
        else res.text().then(text => console.log(res.status, text))
      })
    }
    else { }  //navigator("/not-found", { replace: true })
  })

  const id = (): string | undefined => (
    props.customId === undefined
      ? params.id === "new" ? undefined : params.id
      : props.customId
  )

  const title = () => (
    props.customId === undefined
      ? params.id === "new"
        ? `New ${props.title}`
        : `${props.title} ${params.id}`
      : props.title
  )

  function sendRequest(body: any) {
    fetch(
      `${serverURL}/${props.path}/${id() === undefined ? "" : `${id()}/`}`, {
      method: id() === undefined ? "POST" : "PUT",
      credentials: "include",
      cache: "no-cache",
      headers: {
        "Content-Type": "application/json",
        "authorization": `Bearer ${getToken()}`,
      },
      body: JSON.stringify(body),
    }).then(res => {
      if (res.ok) {
        if (id() === undefined && props.innerList !== undefined) {
          res.json().then(({ id }) => navigator(`/${props.path}/${id}`, { replace: true }))
        } else navigator(-1)
      }
      else res.text().then(text => console.log(res.status, text))
    })
  }

  return (
    <main>
      <Title>RSS: {title()}</Title>
      <Stack
        direction="row"
        justifyContent={"center"}
        spacing={20}
        alignItems="center"
        sx={{
          width: "100%",
          minHeight: "100vh",
          height: "100%",
        }}
      >
        <Stack
          direction="column"
          padding={"32px"}
          spacing={2}
          sx={{ width: props.formWidth }}
        >
          <Typography
            variant="h5"
            textAlign="center"
            sx={{
              pb: "16px",
              textAlign: "center",
            }}
          >
            {title()}
          </Typography>
          <Form
            fields={props.fields}
            data={data()}
            setData={setData}
            buttonText={params.id === "new" ? "Create" : "Update"}
            clearServerError={() => { } /*setServerError(undefined)*/}
            errorCheck={props.errorCheck}
            onSuccess={() => sendRequest(convertData(data(), props.additions))}
          >
            {props.children}
          </Form>
        </Stack>
        {props.innerList !== undefined
          && id() !== undefined
          && <InnerList
            outerPath={props.path}
            outerId={id()}
            width={props.listWidth}
            {...props.innerList}
          />}
      </Stack>
    </main>
  )
}