import { Alert, Stack, Typography } from "@suid/material";
import { Title } from "solid-start";
import { serverError } from "~/components/auth/data";
import SignIn from "~/components/auth/signin";
import SignUp from "~/components/auth/signup";

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
      {serverError() !== undefined
        && <div style={{ position: "absolute", bottom: "30px", right: "20px" }}>
          <Alert severity="error" variant="filled">
            {serverError()}
          </Alert>
        </div>
      }
    </main>
  );
}
