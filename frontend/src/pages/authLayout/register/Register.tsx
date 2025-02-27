import { Modal, Button, Link, Typography, FormInput } from "@/components/UI"

import styles from "../Auth.module.css"
import useInput from "@/hooks/useInput"
import UserService from "@/api/UserService"
import { useNavigate } from "react-router"
import { useUserStore } from "@/state/user"
import { useState } from "react"

export const Register = (): JSX.Element => {
    const [isInvalidData, setIsInvalidData] = useState(false)
    const [isUnuniqueLogin, setIsUnuniqueLogin] = useState(false)
    const [login_prop,] = useInput("")
    const [username_prop,] = useInput("")
    const [password_prop,] = useInput("")
    const navigate = useNavigate()
    const setUserState = useUserStore((state) => state.setUser)
    const setUserAuthState = useUserStore((state) => state.setAuth)

    const handleSubmit = async (e: React.FormEvent<HTMLFormElement>) => {
        e.preventDefault()
        setIsUnuniqueLogin(false)
        setIsInvalidData(false)
        const status = await UserService.createOne({
            login: login_prop.value,
            username: username_prop.value,
            password: password_prop.value
        })
        if (!status) {
            setIsInvalidData(true)
            return
        }
        if (status.message === "Not unique") {
            setIsUnuniqueLogin(true)
            return
        }
        const data = await UserService.getByLogin(login_prop.value)
        if (!data) return
        setUserState(data)
        setUserAuthState(true)
        navigate("/")
    }

    return (
        <Modal className={styles.modal_login}>
            <Typography tag="h1" variant="title-1">Sing in</Typography>
            <form action="POST" className="form sing-in" onSubmit={handleSubmit}>
                <FormInput
                    className={isInvalidData ? styles.invalid : ""}
                    type="text"
                    placeholder="Login..."
                    invalidData={isInvalidData || isUnuniqueLogin}
                    textOnInvalidData={isUnuniqueLogin ? "Login already exists" : "Invalid data"}
                    {...login_prop}
                />
                <FormInput
                    className={isInvalidData ? styles.invalid : ""}
                    type="text"
                    placeholder="Username..."
                    invalidData={isInvalidData}
                    textOnInvalidData="Invalid data"
                    {...username_prop}
                />
                <FormInput
                    className={isInvalidData ? styles.invalid : ""}
                    type="password"
                    placeholder="Password..."
                    invalidData={isInvalidData}
                    textOnInvalidData="Invalid data"
                    {...password_prop}
                />
                <Button
                    className={styles.button}
                    type="submit"
                    disabled={login_prop.value === "" || username_prop.value === "" || password_prop.value === ""}
                >Sing up</Button>
                <Link href="/login" className={styles.link}>or sing in</Link>
            </form>
        </Modal>
    )
}
