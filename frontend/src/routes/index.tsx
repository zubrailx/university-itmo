import { Stack, Typography } from "@suid/material";
import { Title } from "solid-start";
import SignUp from "~/components/auth/signup";
import SignIn from "~/components/auth/signin";

export default function Home() {
  return (
    <main>
      <Title>RSS: Restaurant Supply System</Title>
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
          sx={{
            width: "420px",
            height: "634px",
            borderRadius: "16px",
            border: "1px solid #E6E6E6",
            position: "relative",
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
            New Account
          </Typography>
          <SignUp />
        </Stack>
        <Stack
          direction="column"
          padding={"32px"}
          spacing={2}
          sx={{
            width: "420px",
            height: "334px",
            borderRadius: "16px",
            border: "1px solid #E6E6E6",
            position: "relative",
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
            Log In
          </Typography>
          <SignIn />
        </Stack>
      </Stack>
    </main>
  );
}
