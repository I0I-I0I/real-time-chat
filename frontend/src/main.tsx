import { StrictMode } from "react"
import { createRoot } from "react-dom/client"
import { BrowserRouter, Routes, Route } from "react-router"

import ChatsPage from "@/pages/chats/ChatsPage"
import PageNotFound from "@/pages/pageNotFound/PageNotFound"
import AuthLayout from "@/pages/authLayout/AuthLayout"
import { Login } from "@/pages/authLayout/login/Login"
import { Register } from "@/pages/authLayout/register/Register"
import "./styles/index.css"

createRoot(document.getElementById('root')!).render(
    <StrictMode>
        <BrowserRouter>
            <Routes>
                <Route index element={<ChatsPage />} />
                <Route element={<AuthLayout />}>
                    <Route path="login" element={<Login />} />
                    <Route path="register" element={<Register />} />
                </Route>
                <Route path="*" element={<PageNotFound />} />
            </Routes>
        </BrowserRouter>
    </StrictMode>
)
