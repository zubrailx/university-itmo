import { Navigator, useNavigate } from "@solidjs/router";
import { Stack, Typography } from "@suid/material";
import { createEffect, createSignal } from "solid-js";
import { Title, useParams } from "solid-start";
import UnitInput from "~/components/common/units";
import { convertData } from "~/components/form/data";
import Form from "~/components/form/form";
import { FieldMeta, FormData } from "~/components/form/types";
import { serverURL } from "~/data/fetcher";

enum FormFieldKeys {
  NAME = "name",
  DESCRIPTION = "description",
}

const [data, setData] = createSignal<FormData<FormFieldKeys>>({} as FormData<FormFieldKeys>)

const fields: FieldMeta<FormFieldKeys>[] = [
  { label: "Name", required: true, key: FormFieldKeys.NAME },
  { label: "Description", required: false, key: FormFieldKeys.DESCRIPTION },
]

const [unit, setUnit] = createSignal<string>("")
const [unitError, setUnitError] = createSignal<boolean>(false)

function sendRequest(args: { id?: string, body: any, navigator: Navigator }) {
  fetch(
    `${serverURL}/ingredients/${args.id === undefined ? "" : args.id}`, {
    method: args.id === undefined ? "POST" : "PUT",
    credentials: "include",
    cache: "no-cache",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify(args.body),
  }).then(res => {
    if (res.ok) args.navigator(-1)
    else res.text().then(text => console.log(res.status, text))
  })
}

export default function UserPage() {
  const params = useParams()
  const navigator = useNavigate()

  createEffect(() => {
    if (params.id === "new") { }
    else if (/^[0-9]+$/g.test(params.id)) {
      fetch(
        `${serverURL}/ingredients/${params.id}/`, {
        method: "GET",
        credentials: "include",
      }).then(res => {
        if (res.ok) res.json().then(data => {
          setData({ name: { value: data.name }, description: { value: data.description } })
          setUnit(data.unit.name)
        })
        else if (res.status === 404) navigator("/not-found", { replace: true })
        else res.text().then(text => console.log(res.status, text))
      })
    }
    else navigator("/not-found", { replace: true })
  })

  const id = (): string | undefined => params.id === "new" ? undefined : params.id

  const title = () => (
    params.id === "new"
      ? "New Ingredient"
      : `Ingredient ${params.id}`
  )

  return (
    <main>
      <Title>RSS: {title()}</Title>
      <Stack
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
          sx={{
            width: "420px",
          }}
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
            fields={fields}
            data={data}
            setData={setData}
            buttonText={params.id === "new" ? "Create" : "Update"}
            clearServerError={() => { } /*setServerError(undefined)*/}
            errorCheck={() => {
              const error = !!!unit()
              setUnitError(error)
              return error
            }}
            onSuccess={() => sendRequest({ id: id(), body: convertData(data(), [["unit", unit()]]), navigator })}
          >
            <UnitInput
              width="356px"  // 420 - padding (64)
              unit={unit}
              setUnit={setUnit}
              error={unitError()}
              clearError={() => setUnitError(false)}
            />
          </Form>
        </Stack>
      </Stack>
    </main>
  )
}
