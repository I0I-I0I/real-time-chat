import { Input, Modal, Button, Link, Typography } from "@/components/UI"

import styles from "../Auth.module.css"
import useInput from "@/hooks/useInput"
import { useNavigate, useOutletContext } from "react-router"
import UserService from "@/api/UserService"

export const Login = (): JSX.Element => {
	const [login_prop,] = useInput("")
	const [password_prop,] = useInput("")
    const navigate = useNavigate()
    const setUserState = useOutletContext() as any;

	const handleSubmit = async (e: React.FormEvent<HTMLFormElement>) => {
		e.preventDefault()
        const status = await UserService.checkOne({
            login: login_prop.value,
            password: password_prop.value
        })
        const user = await UserService.getByLogin(login_prop.value)
        if (status) {
            navigate("/")
            if (!user) return
            setUserState(user.login, user.username)
        }
	}

	return (
        <Modal className={styles.modal_login}>
            <Typography tag="h1" variant="title-1">Sing in</Typography>
            <form action="POST" className="form sing-in" onSubmit={handleSubmit}>
                <Input
                    className="input login__input"
                    type="text"
                    placeholder="Login..."
                    {...login_prop}
                />
                <Input
                    className="input login__input"
                    type="password"
                    placeholder="Password..."
                    {...password_prop}
                />
                <Button className={styles.button} type="submit">Sing in</Button>
                <Link href="/register" className={styles.link}>or sing up</Link>
            </form>
        </Modal>
	)
}
