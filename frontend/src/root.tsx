import { AppBar, createTheme, CssBaseline, ThemeProvider } from "@suid/material";
import { Suspense } from "solid-js";
import {
  A,
  Body,
  ErrorBoundary,
  FileRoutes,
  Head,
  Html,
  Link,
  Meta,
  Routes,
  Scripts,
  Title
} from "solid-start";

const theme = createTheme({
  palette: {
    mode: "dark",
  },
});

export default function Root() {
  return (
    <Html lang="en">
      <Head>
        <Title>SolidStart</Title>
        <Meta charset="utf-8" />
        <Meta name="viewport" content="width=device-width, initial-scale=1" />
        <Meta name="theme-color" content="#000000" />
        <Link
          href="https://fonts.googleapis.com/css?family=Roboto:300,400,500"
          rel="stylesheet"
        />
      </Head>
      <Body>
        <Suspense>
          <ErrorBoundary>
            <ThemeProvider theme={theme}>
              <CssBaseline />
              <AppBar>
                <A href="/">Main</A>
              </AppBar>
              <Routes>
                <FileRoutes />
              </Routes>
            </ThemeProvider>
          </ErrorBoundary>
        </Suspense>
        <Scripts />
      </Body>
    </Html>
  );
}
