import { createSignal } from "solid-js";
import IngredientsInput from "~/components/common/ingredients";
import UnitInput from "~/components/common/units";
import FormPage from "~/components/form/page";
import { FieldMeta } from "~/components/form/types";

type Price = {
  nominal_amount: number,
  nominal_cost: number,
  min_count: number,
  max_count: number,
  ingredient: { name: string },
}

enum FormFieldKeys {
  NOMINAL_AMOUNT = "nominal_amount",
  NOMINAL_COST = "nominal_cost",
  MIN_COUNT = "min_count",
  MAX_COUNT = "max_count",
}

const fields: FieldMeta<FormFieldKeys>[] = [
  { label: "Nominal Amount", required: true, key: FormFieldKeys.NOMINAL_AMOUNT, type: "number" },
  { label: "Nominal Cost", required: true, key: FormFieldKeys.NOMINAL_COST, type: "number" },
  { label: "Min Count", required: true, key: FormFieldKeys.MIN_COUNT, type: "number" },
  { label: "Max Count", required: true, key: FormFieldKeys.MAX_COUNT, type: "number" },
]

const [ingredient, setIngredient] = createSignal<{ name: string, id: number }>({ name: "", id: -1 })
const [error, setError] = createSignal<boolean>(false)

export default function UserPage() {
  return <FormPage
    title="Price"
    path="prices"
    formWidth="420px"
    fields={fields}
    unpackData={(data) => { setIngredient(data.ingredient) }}
    errorCheck={() => {
      const error = !!!ingredient() || !!!ingredient().name || ingredient().id < 0
      setError(error)
      return error
    }}
    additions={[["ingredient_id", ingredient().id]]}
  >
    <IngredientsInput
      width="356px"  // 420 - padding (64)
      ingredient={ingredient}
      setIngredient={setIngredient}
      error={error()}
      clearError={() => setError(false)}
    />
  </FormPage>
}
