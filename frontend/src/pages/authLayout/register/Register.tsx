import { Input, Modal, Button, Link, Typography } from "@/components/UI"

import styles from "../Auth.module.css"
import useInput from "@/hooks/useInput"
import UserService from "@/api/UserService"
import { useNavigate, useOutletContext } from "react-router"

export const Register = (): JSX.Element => {
	const [login_prop,] = useInput("")
	const [username_prop,] = useInput("")
	const [password_prop,] = useInput("")
    const navigate = useNavigate()
    const setUserState = useOutletContext() as any;

	const handleSubmit = async (e: React.FormEvent<HTMLFormElement>) => {
		e.preventDefault()
        const status = await UserService.createOne({
            login: login_prop.value,
            username: username_prop.value,
            password: password_prop.value
        })
        if (!status) return
        setUserState(login_prop.value, username_prop.value)
        navigate("/")
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
                    type="text"
                    placeholder="Username..."
                    {...username_prop}
                />
                <Input
                    className="input login__input"
                    type="password"
                    placeholder="Password..."
                    {...password_prop}
                />
                <Button className={styles.button} type="submit">Sing up</Button>
                <Link href="/login" className={styles.link}>or sing in</Link>
            </form>
        </Modal>
	)
}
