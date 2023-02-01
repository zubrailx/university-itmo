import { Stack } from "@suid/material";
import { createEffect } from "solid-js";
import { Title } from "solid-start";
import IngredientsList from "~/components/common/ingredients";
import { getToken } from "~/data/user-store";

export default function Test() {
  createEffect(() => { console.log(getToken()) })
  return (
    <main>
      <Title>RSS: Ingredients</Title>
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
            width: "580px",
          }}
        >
          <IngredientsList />
        </Stack>
      </Stack>
    </main>
  )
}
