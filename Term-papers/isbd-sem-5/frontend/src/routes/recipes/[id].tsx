import { Button, Checkbox, Stack, TextField, Typography } from "@suid/material"
import { createEffect, createSignal } from "solid-js";
import { LinkDialogProps } from "~/components/common/inner-list";
import FormPage from "~/components/form/page";
import { FieldMeta } from "~/components/form/types";

type Ingredient = {
  id: number,
  name: string,
  description?: string,
  unit: { id: number, name: string }
}

type IngredientLink = {
  item: number,
  amount?: number,
  required?: boolean,
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

function Dialog(props: LinkDialogProps<Ingredient, IngredientLink>) {
  const [amount, setAmount] = createSignal<string>("1")
  const [amountError, setAmountError] = createSignal<boolean>(false)
  const [required, setRequired] = createSignal<boolean>(false)

  createEffect(() => props.link?.amount ? setAmount(props.link.amount.toString()) : undefined)
  createEffect(() => props.link?.required ? setRequired(props.link.required) : undefined)

  return <Stack direction="column" alignItems="center" spacing={1}>
    <Typography variant="h6">
      Edit Link
    </Typography>
    <TextField
      label="Amount"
      variant="outlined"
      fullWidth
      value={amount()}
      error={amountError()}
      onChange={({ target: { value } }: any) => {
        setAmount(value.replaceAll(/[^\-0-9]/g, "").substring(0, 10))
        setAmountError(false)
      }}
    />
    <Stack alignItems="center" width="100%" direction="row" onClick={() => setRequired(!!!required())}>
      <Checkbox checked={required()} />
      Required
    </Stack>
    <Stack alignItems="center" width="100%" spacing={2} direction="row">
      <Button
        color="error"
        variant="contained"
        fullWidth
        onClick={() => props.editLink()}
      >
        Delete
      </Button>
      <Button
        variant="contained"
        fullWidth
        onClick={() => {
          if (!!!amount()) setAmountError(true)
          else props.editLink({ item: props.item?.id || -23, ...props.link, amount: parseInt(amount()), required: required() })
        }}
      >
        Save
      </Button>
    </Stack>
  </Stack>
}

export default function MyCafePage() {
  return <>
    <FormPage
      title="Recipe"
      path="recipes"
      formWidth="420px"
      listWidth="580px"
      fields={fields}
      innerList={{
        path: "ingredients",
        extractText: (item: Ingredient) => `${item.name} (${item.unit.name})`,
        linkDialog: Dialog,
      }}
    />
  </>
}
