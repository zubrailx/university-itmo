import { createSignal } from "solid-js";
import FormPage from "~/components/form/page";
import { FieldMeta } from "~/components/form/types";

type Ingredient = {
  id: number,
  name: string,
  description?: string,
  unit: { id: number, name: string }
}

enum FormFieldKeys {
  NAME = "name",
  DESCRIPTION = "description",
  CALORIES = "calories",
}

const fields: FieldMeta<FormFieldKeys>[] = [
  { label: "Name", required: true, key: FormFieldKeys.NAME },
  { label: "Description", required: false, key: FormFieldKeys.DESCRIPTION },
  { label: "Calories", required: false, key: FormFieldKeys.CALORIES, type: "number" },
]

type Menu = {
  name: string,
  description?: string
}

const [choices, setChoices] = createSignal<number[]>([])

export default function MyCafePage() {
  return <>
    <FormPage
      title="Recipe"
      path="recipes"
      formWidth="420px"
      listWidth="540px"
      fields={fields}
      innerList={{
        path: "ingredients",
        choices,
        setChoices,
        extractText: (item: Ingredient) => `${item.name} (${item.unit.name})`,
      }}
    />
  </>
}
