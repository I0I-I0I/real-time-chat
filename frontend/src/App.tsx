import { useEffect } from "react"
import AuthService from "./api/AuthService"
import { useUserStore } from "./state/user"
import { BrowserRouter, Routes, Route } from "react-router"

import ChatsPage from "@/pages/chats/ChatsPage"
import PageNotFound from "@/pages/pageNotFound/PageNotFound"
import AuthLayout from "@/pages/authLayout/AuthLayout"
import { Login } from "@/pages/authLayout/login/Login"
import { Register } from "@/pages/authLayout/register/Register"

export const App = () => {
    const setCurrentUser = useUserStore((state) => state.setUser)
    const setAuth = useUserStore((state) => state.setAuth)

    useEffect(() => {
        const hash = localStorage.getItem("token")
        if (!hash) return
        ;(async () => {
            const response = await AuthService.loginWithHash(hash)
            if (response) {
                setCurrentUser(response)
                setAuth(true)
            }
        })()
    }, [])

    return (
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
    )
}
