import { FieldMeta } from "../form/types"

export type UserData = {
  name: string,
  address?: string,
  description?: string,
}

export enum UserFormFieldKeys {
  NAME = "name",
  DESCRIPTION = "description",
  ADDRESS = "address",
}

export const userFields: FieldMeta<UserFormFieldKeys>[] = [
  { label: "Name", required: true, key: UserFormFieldKeys.NAME },
  { label: "Description", required: false, key: UserFormFieldKeys.DESCRIPTION },
  { label: "Address", required: false, key: UserFormFieldKeys.ADDRESS },
]
