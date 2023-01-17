import { createSignal } from "solid-js";
import FormPage from "~/components/form/page";
import { FieldMeta } from "~/components/form/types";


enum FormFieldKeys {
  NAME = "name",
  DESCRIPTION = "description",
}

const fields: FieldMeta<FormFieldKeys>[] = [
  { label: "Name", required: true, key: FormFieldKeys.NAME },
  { label: "Description", required: false, key: FormFieldKeys.DESCRIPTION },
]

type Menu = {
  id: number
  name: string,
  description?: string
}

type Recipe = {
  id: number
  name: string,
  description: string,
  calories: number,
}

const [choices, setChoices] = createSignal<number[]>([])

export default function MyCafePage() {
  return <>
    <FormPage
      title="Menu"
      path="menus"
      formWidth="420px"
      listWidth="540px"
      fields={fields}
      innerList={{
        path: "recipes",
        choices,
        setChoices,
        extractText: (item: Recipe) => item.calories ? `${item.name} [${item.calories} cal]` : item.name,
      }}
    />
  </>
}
