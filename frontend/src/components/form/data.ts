import { FieldValue, FormData } from '~/components/form/types';

export function convertData<K extends string>(
  data: FormData<K>,
  additions: [string, string | boolean | undefined][] = [],
): object {
  return Object.fromEntries(
    (Object.entries<FieldValue>(data)
      .map(([key, { value }]) => [key, value]) as [string, string | boolean | undefined][])
      .concat(additions)
      .filter(([_, value]) => !!value)
  )
}
