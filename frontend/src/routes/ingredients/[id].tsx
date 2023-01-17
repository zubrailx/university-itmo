import { createSignal } from "solid-js";
import UnitInput from "~/components/common/units";
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

const [unit, setUnit] = createSignal<string>("")
const [unitError, setUnitError] = createSignal<boolean>(false)

export default function UserPage() {
  return <FormPage
    title="Ingredient"
    path="ingredients"
    formWidth="420px"
    fields={fields}
    unpackData={(data) => setUnit(data.unit.name)}
    errorCheck={() => {
      const error = !!!unit()
      setUnitError(error)
      return error
    }}
    additions={[["unit", unit()]]}
  >
    <UnitInput
      width="356px"  // 420 - padding (64)
      unit={unit}
      setUnit={setUnit}
      error={unitError()}
      clearError={() => setUnitError(false)}
    />
  </FormPage>
}
